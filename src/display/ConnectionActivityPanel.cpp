/*
 * ConnectionActivityPanel.cpp
 *
 *  Created on: 26 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "ConnectionActivityPanel.h"

using namespace cryomesh::components;

namespace cryo {

namespace viewer {

namespace display {

ConnectionActivityPanel::ConnectionActivityPanel(const boost::shared_ptr< Connection > & con) : component(con) {
	// TODO Auto-generated constructor stub

}

ConnectionActivityPanel::~ConnectionActivityPanel() {
	// TODO Auto-generated destructor stub
}

const  boost::shared_ptr< cryomesh::components::Connection > & ConnectionActivityPanel::getComponent()const{
	return component;
}

}//NAMESPACE
}//NAMESPACE
}//NAMESPACE
