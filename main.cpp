#include <vector>
#include "rss_gui.hpp"
#include <QApplication>
#include "rss_parser.hpp"
#include "rss_grabber.hpp"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <thread>


void process_img(std::string &&link) {

	//TODO: This is nearly the same code repeated, refactor please.

	const char *path = "cache/";
	mkdir(path, 0777);

	std::cout << link << std::endl;

	std::string t_link = link.substr(0, link.length() - 1);
	std::string name = path + t_link.substr(t_link.find_last_of('/')+1);

	std::cout << name << std::endl;
	std::ifstream ifs(name);
	std::string xml;

	if (!ifs.good()) {

		//This page has not ben cached.
		xml = rss_grabber::grab_xml(link.c_str());
		ifs.close();

		if (xml.empty()) {
			process_img(std::move(link));
			return;
		}

		//Cache the text for next time.
		std::ofstream ofs(name);
		ofs << xml;
		ofs.close();
	} else {

		//This page has been chached, read it into a string.
		std::getline(ifs, xml, char(-1));
		ifs.close();

		if (xml.empty()) {
			unlink(name.c_str());
			process_img(std::move(link));
			return;
		}
	}
	
	//Parse the link.
	assert(!xml.empty());
	link = rss_parser::parse_img(xml);

	t_link = link.substr(0, link.length() - 1);
	name = path + link.substr(link.find_last_of('/')+1);

	ifs.open(name);

	if (!ifs.good()) {

		//grab the image and save it as this name.
		xml = rss_grabber::grab_xml(link.c_str());
		std::ofstream ofs(name);
		ofs << xml;
		ofs.close();

	} //else we already have this image, nevermind.
}

void process_img_list(const std::vector<rss_item> &list) {

	for (const auto &l: list) {
		std::string link;
		link.assign(l.link.begin(), l.link.end());
		process_img(std::move(link));
	}
}

int main(int argc, char **argv) {

	QApplication app(argc, argv);
	rss_gui window;

	std::vector<rss_item> items;

	if (argc < 2) {
		
		std::cout << "Pass the url of the RSS feed as the first argument." << std::endl;
		
		items = rss_parser::parse_file("rss.xml");
	} else {

		//This is ok until we have a settings window.
		const char *url = argv[1];

		std::string xml = rss_grabber::grab_xml(url);
		items = rss_parser::parse_xml(xml);
	}


	for (const auto &item : items) {
		window.add_item(item.title);
	}
	std::thread t(process_img_list, items); t.detach();

	window.show();
	return app.exec();
}
