#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetWindowTitle("ofxLiteGUI Demo");
	ofBackground(0, 0, 0);
	ofSetFrameRate(60);
	
	ofxLiteBox* newBox;
	
	liteGroup.setName("Boxes");
	liteGroup.setSpacing(10.0);
	liteGroup.setSortable(true);
	liteGroup.setMaxHeight(175.0);
	liteGroup.setValue("Box 0", "");
	liteGroup.setValue("Box 1", "");
	
	newBox = liteGroup.setValue("Box 2", "");
	newBox->setTriggerable(true);
	
	newBox = liteGroup.setValue("Box 3", "");
	newBox->configureAsButton();
//	newBox->setDraggable(true);
	
	newBox = liteGroup.setValue("Box 4", "");
	newBox->configureAsToggle();
	newBox->setTriggerable(true);
	
	newBox = liteGroup.setValue("Box 5", "");
	newBox->configureAsDraggableBox();
	
	liteGroup.setValue("Box 6", "");
	liteGroup.setValue("Box 7", "");
	liteGroup.setValue("Box 8", "");
	liteGroup.setValue("Box 9", "");
	
	ofAddListener(liteGroup.boxEvents.boxDragged, this, &testApp::liteBoxDragged);
	ofAddListener(liteGroup.boxEvents.boxDragHovered, this, &testApp::liteBoxDragHovered);
	ofAddListener(liteGroup.boxEvents.boxHovered, this, &testApp::liteBoxHovered);
	ofAddListener(liteGroup.boxEvents.boxPressed, this, &testApp::liteBoxPressed);
	ofAddListener(liteGroup.boxEvents.boxSelected, this, &testApp::liteBoxSelected);
	ofAddListener(liteGroup.boxEvents.boxTriggered, this, &testApp::liteBoxTriggered);
	ofAddListener(liteGroup.groupEvents.groupSorted, this, &testApp::liteGroupSorted);
}

//--------------------------------------------------------------
void testApp::update(){
	liteGroup.update();

}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0, 255, 128);
	liteGroup.draw(20.0, 20.0);
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
	liteGroup.mouseMoved(x, y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	liteGroup.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	liteGroup.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	liteGroup.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

