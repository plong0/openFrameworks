#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetWindowTitle("ofxLiteGUI Demo");
	ofBackground(0, 0, 0);
	ofSetFrameRate(60);
	
	ofxLiteBox* newBox;
	ofxLiteGroup* newGroup;
	
	newGroup = new ofxLiteGroup("Sortable Boxes");
	newGroup->setSpacing(10.0);
	newGroup->setSortable(true);
	newGroup->setMaxHeight(175.0);
	newGroup->setValue("Box 0", "");
	newGroup->setValue("Box 1", "");
	newGroup->setValue("Box 2", "");
	newGroup->setValue("Box 3", "");
	newGroup->setValue("Box 4", "");
	newGroup->setValue("Box 5", "");
	newGroup->setValue("Box 6", "");
	newGroup->setValue("Box 7", "");
	newGroup->setValue("Box 8", "");
	newGroup->setValue("Box 9", "");
	gui.addBox("Sortable Boxes", newGroup);
	
	newGroup = new ofxLiteGroup("Buttons");
	newGroup->setMaxHeight(175.0);
	newGroup->setValue("Button 0", "")->configureAsButton();
	newGroup->setValue("Button 1", "")->configureAsButton();
	newGroup->setValue("Button 2", "")->configureAsButton();
	newGroup->setValue("Button 3", "")->configureAsButton();
	
	newGroup->setValue("Toggle 0", "")->configureAsToggle();
	newGroup->setValue("Toggle 1", "")->configureAsToggle();
	newGroup->setValue("Toggle 2", "")->configureAsToggle();
	newGroup->setValue("Toggle 3", "")->configureAsToggle();
	gui.addBox("Buttons", newGroup);
	
	newGroup = new ofxLiteGroup("Radio Set");
	newGroup->configureAsRadioSet();
	newGroup->setValue("Radio 0", "0");
	newGroup->setValue("Radio 1", "1");
	newGroup->setValue("Radio 2", "2");;
	newGroup->setValue("Radio 3", "3");;
	gui.addBox("Radios", newGroup);
	
	gui.setMaxHeight(175.0);
	gui.setSortable(true);
	
	ofAddListener(gui.boxEvents.boxDragged, this, &testApp::liteBoxDragged);
	ofAddListener(gui.boxEvents.boxDragHovered, this, &testApp::liteBoxDragHovered);
	ofAddListener(gui.boxEvents.boxHovered, this, &testApp::liteBoxHovered);
	ofAddListener(gui.boxEvents.boxPressed, this, &testApp::liteBoxPressed);
	ofAddListener(gui.boxEvents.boxSelected, this, &testApp::liteBoxSelected);
	ofAddListener(gui.boxEvents.boxTriggered, this, &testApp::liteBoxTriggered);
	ofAddListener(gui.groupEvents.groupSingleSelected, this, &testApp::liteGroupSingleSelected);
	ofAddListener(gui.groupEvents.groupSorted, this, &testApp::liteGroupSorted);
}

//--------------------------------------------------------------
void testApp::update(){
	gui.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255, 64, 0);
	gui.draw(20.0, 20.0);
}

//--------------------------------------------------------------
void testApp::liteBoxDragged(ofxLiteEventBoxDragged& event){
	if(!event.isDragging)
		cout << event.liteName << ": " << (event.isDragging?"dragging at":"dragged to") << " (" << event.draggedTo.x << "," << event.draggedTo.y << ")" << endl;
}

//--------------------------------------------------------------
void testApp::liteBoxDragHovered(ofxLiteEventBoxDragHovered& event){
	cout << event.liteName << ": dragged " << (event.isHovered?"on":"off") << " (" << event.hoveredAt.x << "," << event.hoveredAt.y << ")" << endl;
}

//--------------------------------------------------------------
void testApp::liteBoxHovered(ofxLiteEventBoxHovered& event){
	cout << event.liteName << ": " << (event.isHovered?"hovered":"unhovered") << " at (" << event.hoveredAt.x << "," << event.hoveredAt.y << ")" << endl;
}

//--------------------------------------------------------------
void testApp::liteBoxPressed(ofxLiteEventBoxPressed& event){
	cout << event.liteName << ": " << (event.isPressed?"pressed":"released") << " at (" << event.pressedAt.x << "," << event.pressedAt.y << ")" << endl;
}

//--------------------------------------------------------------
void testApp::liteBoxSelected(ofxLiteEventBoxSelected& event){
	cout << event.liteName << ": " << (event.isSelected?"selected":"unselected") << endl;
}

//--------------------------------------------------------------
void testApp::liteBoxTriggered(ofxLiteEventBoxTriggered& event){
	cout << event.liteName << ": triggered" << endl;
}

//--------------------------------------------------------------
void testApp::liteGroupSingleSelected(ofxLiteEventGroupSingleSelected& event){
	cout << event.liteName << ": selected: " << ((event.selected != NULL)?event.selected->getValue():"NULL") << endl;
}

//--------------------------------------------------------------
void testApp::liteGroupSorted(ofxLiteEventGroupSorted& event){
	cout << event.liteName << ": sorted...";
	if(event.oldPosition != -1 && event.newPosition != -1)
		cout << " (moved " << event.oldPosition << " to " << event.newPosition << ")";
	cout << endl;
/**	for(int i=0; i < event.sortedBoxes->size(); i++)
		cout << "\t" << i << "\t" << event.sortedBoxes->at(i)->getName() << endl;
	cout << endl;*/
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	gui.mouseMoved(x, y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	gui.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	gui.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	gui.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

