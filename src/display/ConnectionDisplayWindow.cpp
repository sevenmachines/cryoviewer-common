/*
 * ConnectionDisplayWindow.cpp
 *
 *  Created on: 26 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "ConnectionDisplayWindow.h"
#include "components/Node.h"
namespace cryo {

namespace viewer {

namespace display {

ConnectionDisplayWindow::ConnectionDisplayWindow(const boost::shared_ptr<cryomesh::components::Node> & nd) :
	node(nd) {
	loadWindow("Data/connectiondisplaywindow.glade");
	this->initialise();
	mainWindow->show_all();
}

ConnectionDisplayWindow::~ConnectionDisplayWindow() {
}

void ConnectionDisplayWindow::initialise() {
	// get pointers from glade
	{
		builder->get_widget("connectionDisplayInputsVBox", connectionDisplayInputsVBox);
		builder->get_widget("connectionDisplayOutputsVBox", connectionDisplayOutputsVBox);

	}
	// create all the connections for the node

}

void ConnectionDisplayWindow::updateData() {
	std::cout << "ConnectionDisplayWindow::updateData: " << "TODO" << std::endl;
}

void ConnectionDisplayWindow::updateInputComponentDisplay() {
	this->updateComponentDisplay(node->getConnector().getInputs(), inputPanelsMap, INPUT_CONNECTIONS_PANEL);

}
void ConnectionDisplayWindow::updateOutputComponentDisplay() {
	this->updateComponentDisplay(node->getConnector().getOutputs(), outputPanelsMap, OUTPUT_CONNECTIONS_PANEL);
}

void ConnectionDisplayWindow::updateComponentDisplay(
		const std::map<boost::uuids::uuid, boost::shared_ptr<cryomesh::components::Connection> > & all_components,
		std::map<boost::uuids::uuid, boost::shared_ptr<ConnectionActivityPanel> > & panelsMap,
		const ComponentType & type) {

	std::map<boost::uuids::uuid, boost::shared_ptr<ConnectionActivityPanel> > drawing_areas_copy(panelsMap);

	int count_added = 0;

	// forall in all_components
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<cryomesh::components::Connection> >::const_iterator
				it_all_components = all_components.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<cryomesh::components::Connection> >::const_iterator
				it_all_components_end = all_components.end();
		while (it_all_components != it_all_components_end) {
			boost::shared_ptr<ConnectionActivityPanel> found_panel ;
			if (type == INPUT_CONNECTIONS_PANEL) {
				found_panel = this->findComponentPanelByUUID(it_all_components->second, inputPanelsMap);
			} else if (type == OUTPUT_CONNECTIONS_PANEL) {
				found_panel = this->findComponentPanelByUUID(it_all_components->second, outputPanelsMap);
			}

			//if node doesnt exist then add it
			if (found_panel == 0) {

				if (type == INPUT_CONNECTIONS_PANEL) {
					boost::shared_ptr<ConnectionActivityPanel> panel = this->addInputComponent(
							it_all_components->second);
				} else if (type == OUTPUT_CONNECTIONS_PANEL) {
					boost::shared_ptr<ConnectionActivityPanel> panel = this->addOutputComponent(
							it_all_components->second);
				}
				++count_added;
			} else {
				// if it does then delete it from the copy of drawing areas
				drawing_areas_copy.erase(it_all_components->first);
			}
			++it_all_components;
		}
	}

	int count_removed = 0;
	// remove all panels that werent in the cluster node collection
	// forall in drawing_areas_copy
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<ConnectionActivityPanel> >::const_iterator
				it_drawing_areas_copy = drawing_areas_copy.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<ConnectionActivityPanel> >::const_iterator
				it_drawing_areas_copy_end = drawing_areas_copy.end();
		while (it_drawing_areas_copy != it_drawing_areas_copy_end) {
			if (type == INPUT_CONNECTIONS_PANEL) {
				this->removeInputComponent(it_drawing_areas_copy->first);
			} else if (type == OUTPUT_CONNECTIONS_PANEL) {
				this->removeOutputComponent(it_drawing_areas_copy->first);
			}
			++count_removed;
			++it_drawing_areas_copy;
		}
	}

	// check our panels and nodes balance out
	if (this->isActive() == true) {
		mainWindow->show_all();
	}
	if (all_components.size() != panelsMap.size()) {
		std::cout << "ConnectionDisplayWindow::updateComponentDisplay: "
				<< "ERROR: all_components.size() != panelsMap.size(): " << all_components.size() << " != "
				<< panelsMap.size() << std::endl;
		assert(false);
	}

}
boost::shared_ptr<ConnectionActivityPanel> ConnectionDisplayWindow::addInputComponent(
		const boost::shared_ptr<cryomesh::components::Connection> & connection) {
	boost::shared_ptr<ConnectionActivityPanel> temp_panel = this->addComponent(connection,inputPanelsMap);
	if (temp_panel != 0) {
		connectionDisplayInputsVBox->pack_start(*temp_panel);
	}
	return temp_panel;
}

boost::shared_ptr<ConnectionActivityPanel> ConnectionDisplayWindow::addOutputComponent(
		const boost::shared_ptr<cryomesh::components::Connection> & connection) {
	boost::shared_ptr<ConnectionActivityPanel> temp_panel = this->addComponent(connection, inputPanelsMap);
	if (temp_panel != 0) {
		connectionDisplayOutputsVBox->pack_start(*temp_panel);
	}
	return temp_panel;
}

boost::shared_ptr<ConnectionActivityPanel> ConnectionDisplayWindow::addComponent(
		const boost::shared_ptr<cryomesh::components::Connection> & connection,
		std::map<boost::uuids::uuid, boost::shared_ptr<ConnectionActivityPanel>> & panelsMap) {
	boost::shared_ptr<ConnectionActivityPanel> panel(new ConnectionActivityPanel(connection));
	panelsMap[connection->getUUID()] = panel;
	return panel;
}

void ConnectionDisplayWindow::removeInputComponent(const boost::uuids::uuid & uuid) {
	boost::shared_ptr<ConnectionActivityPanel> temp_panel = this->removeComponent(uuid, inputPanelsMap);
	if (temp_panel != 0) {
		connectionDisplayInputsVBox ->remove(*temp_panel);
	}
}

void ConnectionDisplayWindow::removeOutputComponent(const boost::uuids::uuid & uuid) {
	boost::shared_ptr<ConnectionActivityPanel> temp_panel = this->removeComponent(uuid, outputPanelsMap);
	if (temp_panel != 0) {
		connectionDisplayOutputsVBox ->remove(*temp_panel);
	}
}

boost::shared_ptr<ConnectionActivityPanel> ConnectionDisplayWindow::removeComponent(const boost::uuids::uuid & uuid,
		std::map<boost::uuids::uuid, boost::shared_ptr<ConnectionActivityPanel> > & panelsMap) {
	std::map<boost::uuids::uuid, boost::shared_ptr<ConnectionActivityPanel> >::iterator it_found = panelsMap.find(uuid);
	boost::shared_ptr<ConnectionActivityPanel> found;
	if (it_found != panelsMap.end()) {
		found = it_found->second;
		panelsMap.erase(it_found);
	}
	return found;
}

boost::shared_ptr<ConnectionActivityPanel> ConnectionDisplayWindow::findComponentPanelByUUID(
		const boost::shared_ptr<cryomesh::components::Connection> & component,
		std::map<boost::uuids::uuid, boost::shared_ptr<ConnectionActivityPanel> > & panelsMap) {
	boost::shared_ptr<ConnectionActivityPanel> found_panel;
	bool found = false;
	const boost::uuids::uuid searchid = component->getUUID();
	// forall in panelsMap
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<ConnectionActivityPanel> >::iterator it_panelsMap =
				panelsMap.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<ConnectionActivityPanel> >::const_iterator
				it_panelsMap_end = panelsMap.end();
		while (it_panelsMap != it_panelsMap_end && found == false) {
			if (it_panelsMap->second->getComponent()->getUUID() == searchid) {
				found = true;
				found_panel = it_panelsMap->second;
			}
			++it_panelsMap;
		}
	}
	return found_panel;
}

} //NAMESPACE
}//NAMESPACE
}//NAMESPACE