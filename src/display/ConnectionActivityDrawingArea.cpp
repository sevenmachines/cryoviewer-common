/*
 * ConnectionActivityDrawingArea.cpp
 *
 *  Created on: 3 May 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "ConnectionActivityDrawingArea.h"
#include "common/Containers.h"
#include "common/TimeKeeper.h"

using namespace cryomesh::common;
using namespace cryomesh::components;

namespace cryo {

namespace viewer {

namespace display {

const int ConnectionActivityDrawingArea::MAX_FUTURE_TICKS = 100;
ConnectionActivityDrawingArea::ConnectionActivityDrawingArea(const boost::shared_ptr<cryomesh::components::Connection> & obj) :
	connection(obj) {
}


ConnectionActivityDrawingArea::~ConnectionActivityDrawingArea() {

}

void ConnectionActivityDrawingArea::setAsPrimaryInput(){
	std::cout<<"ConnectionActivityDrawingArea::setAsPrimaryInput: "<<""<<std::endl;
	this->setColourScheme(ActivityDrawingArea::PRIMARY_INPUT_COLOUR_SCHEME);
}
void ConnectionActivityDrawingArea::setAsPrimaryOutput(){
	std::cout<<"ConnectionActivityDrawingArea::setAsPrimaryOutput: "<<""<<std::endl;
	this->setColourScheme(ActivityDrawingArea::PRIMARY_OUTPUT_COLOUR_SCHEME);

}

void ConnectionActivityDrawingArea::update() {
	this->showDrawingArea(this->isActivated());
	if (this->isActivated() == true) {
		points.clear();

		// get its activity from this tick to MAX_FUTURE_TICKS
		Cycle current_cycle = TimeKeeper::getTimeKeeper().getCycle();
		int x = 0;
		for (Cycle i = current_cycle; i < current_cycle + MAX_FUTURE_TICKS; i++) {
			//points[x] = connection->get
			//std::cout << "ConnectionActivityDrawingArea::update: " << i << " < " << current_cycle + MAX_FUTURE_TICKS
			//<< " activity: " << points[x] << std::endl;
			++x;
		}
		connection->setDebug(true);
		std::cout << "ConnectionActivityDrawingArea::update: " << "Connection: " << *connection << std::endl;
		connection->setDebug(false);

		//set colour
	}
	this->invalidateWindow();

}

void ConnectionActivityDrawingArea::drawText() {
	ActivityDrawingArea::drawText();
	// This is where we draw on the window
	Glib::RefPtr < Gdk::Window > window = get_window();
	Cairo::RefPtr < Cairo::Context > cr = window->create_cairo_context();
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();
	cr->save();
	cr->set_line_width(2.0);
	cr->set_source_rgb(1, 1, 1);
	cr->set_font_size(18);

	{
		cr->move_to(width / 2, 20);
		std::stringstream ss;
		ss << "ID: " << connection->getUUIDString();
		cr->show_text(ss.str());
	}

	{
		std::stringstream ss;
		cr->move_to(width / 2, 40);
		// 	ss<<"strength: "<<fibril->getMutator().getStrength()
		//    	<<" compression: "<<fibril->getMutator().getCompression()
		//  	<<" delay: " <<fibril->getMutator().getDelay();
		cr->show_text(ss.str());
	}
	cr->restore();
}

void ConnectionActivityDrawingArea::drawAxis() {

	ActivityDrawingArea::drawAxis();
}

}

}

}
