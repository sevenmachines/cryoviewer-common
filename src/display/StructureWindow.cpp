/*
 * StructureWindow.cpp
 *
 *  Created on: 8 Apr 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "NodeDisplayData.h"
#include "StructureWindow.h"
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>
#include <osgGA/TrackballManipulator>
#include <osg/ShapeDrawable>

using namespace cryomesh::components;

namespace cryo {

namespace viewer {

namespace display {

StructureWindow::StructureWindow(const boost::shared_ptr<cryomesh::structures::Bundle> bun) :
	bundle(bun) {

	displayText = osg::ref_ptr<osgText::Text>(new osgText::Text);
	displayText->setText("GOD HELP US ALL!!");
	loadWindow("Data/structurewindow.glade");
	this->setTitle("Cryoviewer Structure");
}

StructureWindow::~StructureWindow() {
}

void StructureWindow::updateData() {
	std::cout<<"StructureWindow::updateData: "<<""<<std::endl;
	if (structureDrawingArea != 0) {
		std::cout<<"StructureWindow::updateData: "<<"structureDrawingArea != 0"<<std::endl;
		structureDrawingArea->update();
	}
	if (activitiesWindow !=0){
		activitiesWindow->update();
	}
}
void StructureWindow::initialise() {

	// set selected elements to some sort of defaults
	selectedCluster = bundle->getClusters().begin()->second;

	// get out widgets
	builder->get_widget("structureVisualiseButton", structureVisualiseButton);
	builder->get_widget("structureVBox", structureVBox);
	builder->get_widget("structureActivitiesToggleButton", structureActivitiesToggleButton);

	// connect up togglebuttons
	structureVisualiseButton->signal_clicked().connect(
			sigc::mem_fun(*this, &StructureWindow::onStructureVisualiseButtonClicked));
	structureActivitiesToggleButton->signal_clicked().connect(
			sigc::mem_fun(*this, &StructureWindow::onStructureActivitiesToggleButtonClicked));

	structureDrawingArea = new StructureGLDrawingArea(bundle);
	//TODO test by setting to first cluster
	structureDrawingArea->setDrawTest();

	structureVBox->pack_start(*structureDrawingArea);
	structureVBox->show();
}

void StructureWindow::onStructureVisualiseButtonClicked() {
	std::cout << "StructureWindow::onStructureVisualiseButtonClicked: " << "TODO: This is a demo" << std::endl;
	const std::map<boost::uuids::uuid, boost::shared_ptr<Node> > & nodes =
			selectedCluster->getNodes();
	this->showVisual(nodes);
}

void StructureWindow::onStructureActivitiesToggleButtonClicked() {
	if (structureActivitiesToggleButton->get_active() == true) {
		if (activitiesWindow == 0) {
			activitiesWindow = boost::shared_ptr<ActivitiesWindow>(new ActivitiesWindow(selectedCluster));
		}
		activitiesWindow->activate();
	} else {
		activitiesWindow->deactivate();
	}
}
void StructureWindow::showVisual(
		const std::map<boost::uuids::uuid, boost::shared_ptr<cryomesh::components::Node> > & nodes) {
	osgViewer::Viewer viewer;
	//viewer.setUpViewInWindow(100,100, 500,500, 1);
	osg::Group* root = new osg::Group();

	// forall in nodes
	{
		std::map<boost::uuids::uuid, boost::shared_ptr<Node> >::const_iterator it_nodes = nodes.begin();
		const std::map<boost::uuids::uuid, boost::shared_ptr<Node> >::const_iterator it_nodes_end = nodes.end();
		while (it_nodes != it_nodes_end) {
			this->addNodeDrawable(it_nodes->second, root);
			++it_nodes;
		}
	}

	// draw text
	root->addChild(this->createHUD(displayText));
	root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	viewer.setSceneData(root);

	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
	viewer.realize();

	while (!viewer.done()) {
		viewer.frame();
	}
}

osg::Node* StructureWindow::createHUD(osgText::Text* updateText) {
	std::cout << "StructureWindow::createHUD: " << "" << std::endl;
	// create the hud. derived from osgHud.cpp
	// adds a set of quads, each in a separate Geode - which can be picked individually
	// eg to be used as a menuing/help system!
	// Can pick texts too!

	osg::Camera* hudCamera = new osg::Camera;
	hudCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	hudCamera->setProjectionMatrixAsOrtho2D(0, 1280, 0, 1024);
	hudCamera->setViewMatrix(osg::Matrix::identity());
	hudCamera->setRenderOrder(osg::Camera::POST_RENDER);
	hudCamera->setClearMask(GL_DEPTH_BUFFER_BIT);

	std::string timesFont("fonts/times.ttf");

	// turn lighting off for the text and disable depth test to ensure its always ontop.
	osg::Vec3 position(150.0f, 800.0f, 0.0f);
	osg::Vec3 delta(0.0f, -60.0f, 0.0f);

	{
		osg::Geode* geode = new osg::Geode();
		osg::StateSet* stateset = geode->getOrCreateStateSet();
		stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
		geode->setName("simple");
		hudCamera->addChild(geode);

		osgText::Text* text = new osgText::Text;
		geode->addDrawable(text);

		text->setFont(timesFont);
		text->setText("Picking in Head Up Displays is simple!");
		text->setPosition(position);

		position += delta;
	}

	for (int i = 0; i < 5; i++) {
		osg::Vec3 dy(0.0f, -30.0f, 0.0f);
		osg::Vec3 dx(120.0f, 0.0f, 0.0f);
		osg::Geode* geode = new osg::Geode();
		osg::StateSet* stateset = geode->getOrCreateStateSet();
		const char *opts[] = { "One", "Two", "Three", "January", "Feb", "2003" };
		osg::Geometry *quad = new osg::Geometry;
		stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
		std::string name = "subOption";
		name += " ";
		name += std::string(opts[i]);
		geode->setName(name);
		osg::Vec3Array* vertices = new osg::Vec3Array(4); // 1 quad
		osg::Vec4Array* colors = new osg::Vec4Array;
		colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(0.8 - 0.1 * i, 0.1 * i, 0.2 * i, 1.0));
		quad->setColorArray(colors);
		quad->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);
		(*vertices)[0] = position;
		(*vertices)[1] = position + dx;
		(*vertices)[2] = position + dx + dy;
		(*vertices)[3] = position + dy;
		quad->setVertexArray(vertices);
		quad->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
		geode->addDrawable(quad);
		hudCamera->addChild(geode);

		position += delta;
	}

	{ // this displays what has been selected
		osg::Geode* geode = new osg::Geode();
		osg::StateSet* stateset = geode->getOrCreateStateSet();
		stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
		geode->setName("The text label");
		geode->addDrawable(updateText);
		hudCamera->addChild(geode);

		updateText->setCharacterSize(20.0f);
		updateText->setFont(timesFont);
		updateText->setColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
		updateText->setText("");
		updateText->setPosition(position);
		updateText->setDataVariance(osg::Object::DYNAMIC);

		position += delta;
	}

	return hudCamera;

}

void StructureWindow::showText(osg::Group* root) const {
	osg::Vec3 position(150.0f, 800.0f, 0.0f);
	osg::Vec3 delta(0.0f, -60.0f, 0.0f);
	osg::Geode* geode = new osg::Geode();
	osg::StateSet* stateset = geode->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	geode->setName("simple");

	geode->addDrawable(displayText);

	displayText->setPosition(position);
	root->addChild(geode);
}

void StructureWindow::addNodeDrawable(const boost::shared_ptr<cryomesh::components::Node> & node, osg::Group* root) const {
	osg::Geometry* nodeGeometry = new osg::Geometry();

	NodeDisplayData * nodeData(new NodeDisplayData(node));
	osg::ShapeDrawable * nodeDrawable(new osg::ShapeDrawable(nodeData->getShape().get()));
	nodeDrawable->setColor(osg::Vec4f(1, 0, 0, 0.5));
	osg::Geode* nodeGeode = new osg::Geode();
	nodeGeode->addDrawable(nodeDrawable);
	root->addChild(nodeGeode);
	// set colour
	/*
	 osg::Vec4Array* colours = new osg::Vec4Array;
	 colours->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f)); //index 0 red
	 colours->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f)); //index 1 green
	 colours->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f)); //index 2 blue
	 colours->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)); //index 3 white
	 colours->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f)); //index 4 red
	 nodeGeometry->setShape(nodeData->getShape().get());
	 nodeGeometry->setColorArray(colours);
	 nodeGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);
	 */
}

void StructureWindow::setLabel(const std::string& name) {
	std::cout << "StructureWindow::setLabel: " << name << std::endl;
	if (displayText.get())
		displayText->setText(name);
}

}//NAMESPACE

}//NAMESPACE

}
