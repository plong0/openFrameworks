#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);
	ofBackground(0, 0, 0);
	ofSetLogLevel(OF_LOG_ERROR);
	
	midiIn.openPort(0);
	
	val1 = val2 = val3 = val4 = 0;
	
	midiMap.setMidiIn(&midiIn);
	midiMap.addControllableValue("Value 1", &val1);
	midiMap.addControllableValue("Value 2", &val2);
	midiMap.addControllableValue("Value 3", &val3);
	midiMap.addControllableValue("Value 4", &val4);
	
	ofAddListener(midiMap.valueUpdate, this, &testApp::midiMapValueChanged);
}

//--------------------------------------------------------------
void testApp::update(){
	midiMap.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255, 255, 255);
	midiMap.draw();
}

//--------------------------------------------------------------
void testApp::midiMapValueChanged(ofxMidiMapValueUpdateEvent &event){
	cout << "updated:" << event.name << ":" << *(event.value) << ":" << endl;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	midiMap.mouseMoved(x, y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	midiMap.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	midiMap.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	midiMap.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

