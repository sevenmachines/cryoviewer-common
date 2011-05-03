/*
 * ConnectionActivityDrawingArea.h
 *
 *  Created on: 3 May 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef CONNECTIONACTIVITYDRAWINGAREA_H_
#define CONNECTIONACTIVITYDRAWINGAREA_H_

#include "ActivityDrawingArea.h"
#include "components/Connection.h"
#include <boost/shared_ptr.hpp>

namespace cryo {

namespace viewer {

namespace display {

class ConnectionActivityDrawingArea: public ActivityDrawingArea {
public:
	ConnectionActivityDrawingArea(const boost::shared_ptr< cryomesh::components::Connection> & obj );
	virtual ~ConnectionActivityDrawingArea();
	void setAsPrimaryInput();
	void setAsPrimaryOutput();
void update();
protected:
	virtual 	void drawText();
	virtual void  drawAxis();
private:
	const boost::shared_ptr< cryomesh::components::Connection> connection;
	static const int MAX_FUTURE_TICKS;
};

}

}

}

#endif /* CONNECTIONACTIVITYDRAWINGAREA_H_ */
