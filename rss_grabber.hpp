#ifndef RSS_GRABBER_HPP
#define RSS_GRABBER_HPP

#include <curl/curl.h>
#include <curl/easy.h>
#include <string>
#include <vector>
#include <map>
#include <functional>

class rss_item;

namespace rss_grabber {

	void process_img_list(const std::map<unsigned, rss_item> &list, std::function<void(unsigned, std::string&&)>&&);
	std::string grab_xml(const char *url);
}
#endif
