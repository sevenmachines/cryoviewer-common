/*
 * ConnectionActivityPanel.h
 *
 *  Created on: 26 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef CONNECTIONACTIVITYPANEL_H_
#define CONNECTIONACTIVITYPANEL_H_

#include <gtkmm/box.h>
#include "components/Connection.h"
#include <boost/shared_ptr.hpp>
namespace cryo {

namespace viewer {

namespace display {

class ConnectionActivityPanel : public Gtk::HBox{
public:
	ConnectionActivityPanel(const boost::shared_ptr< cryomesh::components::Connection > & con);
	virtual ~ConnectionActivityPanel();

	const  boost::shared_ptr< cryomesh::components::Connection > & getComponent()const;

private:
	const boost::shared_ptr< cryomesh::components::Connection > & component;
};

}

}

}

#endif /* CONNECTIONACTIVITYPANEL_H_ */
