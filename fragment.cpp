#include "fragment.hpp"
#include <QDesktopWidget>
#include <QApplication>

fragment::fragment(QWidget *parent) : QWidget(parent) {

	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_X11DoNotAcceptFocus);
	
	this->corner = 15;
	this->colour = QColor(11, 11, 44, 127);
	offset = 0;

}

void fragment::paintEvent(QPaintEvent *e) {

	Q_UNUSED(e);
	QPainter qp(this);
	draw_lines(&qp);

	//TODO: This is a hack.
	QDesktopWidget *desktop = QApplication::desktop();
	int width = desktop->width();
	move((width - this->width())-1-offset, 410);
}

void fragment::draw_lines(QPainter *qp) {

	qp->setOpacity(1.0);
	QPen pen(Qt::NoPen);
	qp->setPen(pen);

 	QBrush brush;
	brush.setStyle(Qt::SolidPattern); 
      	brush.setColor(this->colour);
	qp->setBrush(brush);

	qp->drawRoundedRect(0,0, this->width(), this->height(), this->corner, this->corner);
}
