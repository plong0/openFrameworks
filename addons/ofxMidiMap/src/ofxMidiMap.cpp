/*
 *  ofxMidiMap.cpp
 *
 *  Created by Pat Long (plong0) on 11-03-30.
 *  Copyright 2011 Spiral Sense. All rights reserved.
 *
 */

#include "ofxMidiMap.h"

ofxMidiMap::ofxMidiMap(){
	midiIn = NULL;
	controlledValues.setName("Controlled Values");
	
//	controlledValues.setSortable(true);
	midiValues.setSortable(true);
	
	selectedControlled = NULL;
	selectedMidiControl = NULL;
}

ofxMidiMap::~ofxMidiMap(){
}

void ofxMidiMap::draw(){
	midiValues.draw(20.0, 20.0);
	controlledValues.draw(ofGetWidth()/2.0+20.0, 20.0);
}

void ofxMidiMap::update(){
	if(this->selectedMidiControl != NULL){
		if(!this->selectedMidiControl->isSelected())
			this->selectedMidiControl = NULL;
	}
	
	map<string,ofxLiteBox*>* midiValueBoxes = midiValues.getBoxes();
	for(map<string,ofxLiteBox*>::iterator bIt = midiValueBoxes->begin(); bIt != midiValueBoxes->end(); bIt++){
		MidiControlDisplay* cMidiValue = (MidiControlDisplay*)bIt->second;
		if(cMidiValue != NULL){
			if(cMidiValue->isSelected() && this->selectedMidiControl != cMidiValue){
				if(this->selectedMidiControl != NULL)
					this->selectedMidiControl->setSelected(false);
				this->selectedMidiControl = cMidiValue;
				break;
			}
		}
	}
	
	map<string,ofxLiteBox*>* controlledValueBoxes = controlledValues.getBoxes();
	for(map<string,ofxLiteBox*>::iterator bIt = controlledValueBoxes->begin(); bIt != controlledValueBoxes->end(); bIt++){
		ControlledValueDisplay* cControlValue = (ControlledValueDisplay*)bIt->second;
		if(cControlValue != NULL){
			if(selectedMidiControl != NULL){
				if(cControlValue->isSelected())
					selectedMidiControl->addControlledValue(cControlValue);
				else
					selectedMidiControl->removeControlledValue(cControlValue);
			}
		}
	}
	
/**
	
	for(map<string,ValueDisplay*>::iterator cvIt = controlledValues.values.begin(); cvIt != controlledValues.values.end(); cvIt++){
		ControlledValueDisplay* cControlValue = (ControlledValueDisplay*)cvIt->second;
		if(cControlValue != NULL){
			if(selectedMidiControl != NULL){
				if(cControlValue->isSelected()){
					selectedMidiControl->addControlledValue(cControlValue);
				}
				else{
					selectedMidiControl->removeControlledValue(cControlValue);
				}
			}
			
		}
	}*/
}

void ofxMidiMap::midiMessage(ofxMidiEventArgs &args){
	stringstream strName;
/**	TODO: use this info to set the control type
	switch (args.status){
		case MIDI_NOTE_OFF:
			strName << "nf:";
			break;
		case MIDI_NOTE_ON:
			strName << "nn:";
			break;
		case MIDI_POLY_PRESSURE:
			strName << "pp:";
			break;
		case MIDI_CONTROL_CHANGE:
			strName << "cc:";
			break;
		case MIDI_PROGRAM_CHANGE:
			strName << "pc:";
			break;
		case MIDI_CHANNEL_PRESSURE:
			strName << "cp:";
			break;
		case MIDI_PITCH_BEND:
			strName << "pb:";
			break;
		default:
			break;
	}	*/
	
	strName << args.byteOne;
	
	string name = strName.str();
	string value = ofToString(args.byteTwo, 0);
	strName.str("");
	
	if(midiValues.hasBox(name) > 0)
		midiValues.setValue(name, value);
	else{
		MidiControlDisplay* newValue = new MidiControlDisplay(name, value);
		ofxLiteBox* oldValue = midiValues.addBox(name, newValue);
		if(oldValue != NULL)
			delete oldValue;
	}
}

void ofxMidiMap::addControllableValue(string name, int* value){
	ControlledValueDisplay* newControllableValue = new ControlledValueDisplay(name, value);
	newControllableValue->setMidiMap(this);
	ofxLiteBox* oldValue = controlledValues.addBox(name, newControllableValue);
	if(oldValue != NULL)
		delete oldValue;
}

void ofxMidiMap::setMidiIn(ofxMidiIn* midiIn){
	this->midiIn = midiIn;
	if(midiIn != NULL){
		midiIn->listPorts();
		if(midiIn->portNames.size() > midiIn->getPort())
			midiValues.setName(midiIn->portNames[midiIn->getPort()]);
		ofAddListener(midiIn->newMessageEvent, this, &ofxMidiMap::midiMessage);
	}
}

bool ofxMidiMap::mouseMoved(int x, int y){
	bool result = false;
	if(controlledValues.mouseMoved(x, y))
		result = true;
	if(midiValues.mouseMoved(x, y))
		result = true;
	return result;
}

bool ofxMidiMap::mouseDragged(int x, int y, int button){
	bool result = false;
	if(controlledValues.mouseDragged(x, y, button))
		result = true;
	if(midiValues.mouseDragged(x, y, button))
		result = true;
	return result;
}

bool ofxMidiMap::mousePressed(int x, int y, int button){
	bool result = false;
	if(controlledValues.mousePressed(x, y, button))
		result = true;
	if(midiValues.mousePressed(x, y, button))
		result = true;
	return result;
}

bool ofxMidiMap::mouseReleased(int x, int y, int button){
	bool result = false;
	if(controlledValues.mouseReleased(x, y, button))
		result = true;
	if(midiValues.mouseReleased(x, y, button))
		result = true;
	return result;
}


/*** LinkedBox ***/
ofxMidiMap::LinkedBox::LinkedBox(string name, string value):ofxLiteBox(name, value){
	setPadding(5.0, 25.0, 5.0, 5.0);
}

void ofxMidiMap::LinkedBox::drawLinkBox(bool fill, float x, float y){
	if(x == -1.0) x = bounds.x;
	if(y == -1.0) y = bounds.y;
	
	ofRectangle linkBox;
	linkBox.x = x+5;
	linkBox.y = y+5;
	linkBox.width = bounds.height-10;
	linkBox.height = bounds.height-10;
	ofNoFill();
	ofRect(linkBox.x, linkBox.y, linkBox.width, linkBox.height);
	
	if(fill){
		ofFill();
		ofRect(linkBox.x+2, linkBox.y+2, linkBox.width-4, linkBox.height-4);
	}
}

void ofxMidiMap::LinkedBox::drawContent(float x, float y){	
	ofxLiteBox::drawContent(x, y);
	drawLinkBox(false, x, y);
}


/*** ControlledValueDisplay ***/
ofxMidiMap::ControlledValueDisplay::ControlledValueDisplay(string name, int* controlValue) : ofxMidiMap::LinkedBox(name){
	this->midiMap = NULL;
	this->midiValue = NULL;
	this->controlValue = controlValue;
	setValue(ofToString(*controlValue, 0));
}

void ofxMidiMap::ControlledValueDisplay::drawContent(float x, float y){
	ofxMidiMap::LinkedBox::drawContent(x, y);
	if(midiValue != NULL){
		if(isHovered())
			midiValue->drawHoverBox();
		drawLinkBox(true);
	}
}

ofxMidiMap::MidiControlDisplay* ofxMidiMap::ControlledValueDisplay::getMidiValue(){
	return this->midiValue;
}

void ofxMidiMap::ControlledValueDisplay::setMidiMap(ofxMidiMap* midiMap){
	this->midiMap = midiMap;
}

void ofxMidiMap::ControlledValueDisplay::setMidiValue(ofxMidiMap::MidiControlDisplay* midiValue){
	this->midiValue = midiValue;
}

void ofxMidiMap::ControlledValueDisplay::setSelected(bool selected){
	ofxMidiMap::LinkedBox::setSelected(selected);
	if(selected && midiValue != NULL && !midiValue->isSelected())
		midiValue->setSelected(true);
}

void ofxMidiMap::ControlledValueDisplay::setValue(string value){
	ofxMidiMap::LinkedBox::setValue(value);
	
	(*controlValue) = ofToInt(value);
	
	if(this->midiMap != NULL){
		ofxMidiMapValueUpdateEvent eventArgs;
		eventArgs.name = name;
		eventArgs.value = controlValue;
		ofNotifyEvent(this->midiMap->valueUpdate, eventArgs, this);
	}
}


/*** MidiControlDisplay ***/
ofxMidiMap::MidiControlDisplay::MidiControlDisplay(string name, string value) : ofxMidiMap::LinkedBox(name, value){
}

void ofxMidiMap::MidiControlDisplay::drawContent(float x, float y){
	ofxMidiMap::LinkedBox::drawContent(x, y);
	if(controlledValues.size() > 0){
		drawLinkBox(true);
		if(isHovered()){
			for(map<string,ControlledValueDisplay*>::iterator cvIt = controlledValues.begin(); cvIt != controlledValues.end(); cvIt++){
				ControlledValueDisplay* cValue = cvIt->second;
				if(cValue != NULL){
					cValue->drawHoverBox();
				}
			}
		}
	}
}

bool ofxMidiMap::MidiControlDisplay::addControlledValue(ControlledValueDisplay* controlValue){
	if(controlValue != NULL){
		if(controlValue->getMidiValue() != NULL && controlValue->getMidiValue() != this){
			controlValue->getMidiValue()->removeControlledValue(controlValue);
		}
		
		if(controlledValues.count(controlValue->getName()) == 0){
			controlledValues[controlValue->getName()] = controlValue;
			controlValue->setMidiValue(this);
			controlValue->setValue(value);
		}
	}
}

bool ofxMidiMap::MidiControlDisplay::removeControlledValue(ControlledValueDisplay* controlValue){
	if(controlValue != NULL){
		if(controlledValues.count(controlValue->getName()) > 0){
			controlledValues.erase(controlValue->getName());
			controlValue->setMidiValue(NULL);
		}
	}
}

void ofxMidiMap::MidiControlDisplay::setSelected(bool selected){
	ofxMidiMap::LinkedBox::setSelected(selected);
	for(map<string,ControlledValueDisplay*>::iterator cvIt = controlledValues.begin(); cvIt != controlledValues.end(); cvIt++){
		ControlledValueDisplay* cValue = cvIt->second;
		if(cValue != NULL)
			cValue->setSelected(selected);
	}
}

void ofxMidiMap::MidiControlDisplay::setValue(string value){
	ofxMidiMap::LinkedBox::setValue(value);
	for(map<string,ControlledValueDisplay*>::iterator cvIt = controlledValues.begin(); cvIt != controlledValues.end(); cvIt++){
		ControlledValueDisplay* cValue = cvIt->second;
		if(cValue != NULL)
			cValue->setValue(value);
	}
}
