/*
 * ComponentDisplayData.h
 *
 *  Created on: 8 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef COMPONENTDISPLAYDATA_H_
#define COMPONENTDISPLAYDATA_H_

#include <osg/ShapeDrawable>
#include <boost/shared_ptr.hpp>

namespace cryo {

namespace viewer {

namespace display {

class ComponentDisplayData {
public:
	ComponentDisplayData();
	virtual ~ComponentDisplayData();
	virtual void update()=0;
	virtual  boost::shared_ptr< osg::Shape > getShape()=0;
};

}

}

}

#endif /* COMPONENTDISPLAYDATA_H_ */
