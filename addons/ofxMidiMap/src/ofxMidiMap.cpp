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
		if(!this->selectedMidiControl->isSelected()){
			this->selectedMidiControl = NULL;
		}
	}
	for(map<string,ValueDisplay*>::iterator mvIt = midiValues.values.begin(); mvIt != midiValues.values.end(); mvIt++){
		MidiControlDisplay* cMidiValue = (MidiControlDisplay*)mvIt->second;
		if(cMidiValue != NULL){
			if(cMidiValue->isSelected()){
				if(this->selectedMidiControl != cMidiValue){
					if(this->selectedMidiControl != NULL)
						this->selectedMidiControl->setSelected(false);
					this->selectedMidiControl = cMidiValue;
				}
			}
		}
	}
	
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
	}
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
	
	if(midiValues.values.count(name) > 0)
		midiValues.setValue(name, value);
	else{
		MidiControlDisplay* newValue = new MidiControlDisplay(name, value);
		ValueDisplay* oldValue = midiValues.addValue(name, newValue);
		if(oldValue != NULL)
			delete oldValue;
	}
}

void ofxMidiMap::addControllableValue(string name, int* value){
	ControlledValueDisplay* newControllableValue = new ControlledValueDisplay(name, value);
	newControllableValue->midiMap = this;
	ValueDisplay* oldValue = controlledValues.addValue(name, newControllableValue);
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


/*** ControlledValueDisplay ***/
ofxMidiMap::ControlledValueDisplay::ControlledValueDisplay(string name, int* controlValue) : ofxMidiMap::ValueDisplay(name){
	this->midiMap = NULL;
	this->midiValue = NULL;
	this->controlValue = controlValue;
	setValue(ofToString(*controlValue, 0));
}

void ofxMidiMap::ControlledValueDisplay::draw(float x, float y){
	ofxMidiMap::ValueDisplay::draw(x, y);
	if(midiValue != NULL){
		if(isHovered())
			midiValue->drawHoverBox();
		drawLinkBox(true);
	}
}

void ofxMidiMap::ControlledValueDisplay::setSelected(bool selected){
	ofxMidiMap::ValueDisplay::setSelected(selected);
	if(selected && midiValue != NULL && !midiValue->isSelected())
		midiValue->setSelected(true);
}

void ofxMidiMap::ControlledValueDisplay::setValue(string value){
	ofxMidiMap::ValueDisplay::setValue(value);
	
	(*controlValue) = ofToInt(value);
	
	if(this->midiMap != NULL){
		ofxMidiMapValueUpdateEvent eventArgs;
		eventArgs.name = name;
		eventArgs.value = controlValue;
		ofNotifyEvent(this->midiMap->valueUpdate, eventArgs, this);
	}
}


/*** MidiControlDisplay ***/
ofxMidiMap::MidiControlDisplay::MidiControlDisplay(string name, string value) : ofxMidiMap::ValueDisplay(name, value){
}

void ofxMidiMap::MidiControlDisplay::draw(float x, float y){
	ofxMidiMap::ValueDisplay::draw(x, y);
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
		if(controlValue->midiValue != NULL && controlValue->midiValue != this){
			controlValue->midiValue->removeControlledValue(controlValue);
		}
		
		if(controlledValues.count(controlValue->name) == 0){
			controlledValues[controlValue->name] = controlValue;
			controlValue->midiValue = this;
			controlValue->setValue(value);
		}
	}
}

bool ofxMidiMap::MidiControlDisplay::removeControlledValue(ControlledValueDisplay* controlValue){
	if(controlValue != NULL){
		if(controlledValues.count(controlValue->name) > 0){
			controlledValues.erase(controlValue->name);
			controlValue->midiValue = NULL;
		}
	}
}

void ofxMidiMap::MidiControlDisplay::setSelected(bool selected){
	ofxMidiMap::ValueDisplay::setSelected(selected);
	for(map<string,ControlledValueDisplay*>::iterator cvIt = controlledValues.begin(); cvIt != controlledValues.end(); cvIt++){
		ControlledValueDisplay* cValue = cvIt->second;
		if(cValue != NULL)
			cValue->setSelected(selected);
	}
}

void ofxMidiMap::MidiControlDisplay::setValue(string value){
	ofxMidiMap::ValueDisplay::setValue(value);
	for(map<string,ControlledValueDisplay*>::iterator cvIt = controlledValues.begin(); cvIt != controlledValues.end(); cvIt++){
		ControlledValueDisplay* cValue = cvIt->second;
		if(cValue != NULL)
			cValue->setValue(value);
	}
}


/*** ValueDisplay ***/
ofxMidiMap::ValueDisplay::ValueDisplay(string name, string value){
	this->name = name;
	this->setValue(value);
	bounds.x = bounds.y = bounds.width = bounds.height = 0.0;
	hover = false;
	selected = false;
}

void ofxMidiMap::ValueDisplay::drawHoverBox(){
	ofNoFill();
	ofRect(bounds.x+2, bounds.y+2, bounds.width-4, bounds.height-4);
}

void ofxMidiMap::ValueDisplay::drawLinkBox(bool fill){
	ofRectangle linkBox;
	linkBox.x = bounds.x+5;
	linkBox.y = bounds.y+5;
	linkBox.width = bounds.height-10;
	linkBox.height = bounds.height-10;
	ofNoFill();
	ofRect(linkBox.x, linkBox.y, linkBox.width, linkBox.height);
	
	if(fill){
		ofFill();
		ofRect(linkBox.x+2, linkBox.y+2, linkBox.width-4, linkBox.height-4);
	}
}

void ofxMidiMap::ValueDisplay::drawSelectedBox(){
	ofSetLineWidth(3.0);
	ofNoFill();
	ofRect(bounds.x+1, bounds.y+1, bounds.width-2, bounds.height-2);
	ofSetLineWidth(1.0);
}

void ofxMidiMap::ValueDisplay::draw(float x, float y){
	if(bounds.x != x) bounds.x = x;
	if(bounds.y != y) bounds.y = y;
	
	float width = 0.0;
	float height = 25.0;
	
	stringstream builder;
	builder << name << " [" << value << "]";
	ofDrawBitmapString(builder.str(), bounds.x+25.0, bounds.y+20.0);
	
	width = 20.0+builder.str().length()*10.0;
	
	if(bounds.width != width) bounds.width = width;
	if(bounds.height != height) bounds.height = height;
	
	ofNoFill();
	ofRect(bounds.x, bounds.y, bounds.width, bounds.height);
	
	drawLinkBox(false);

	if(isSelected()){
		drawSelectedBox();
	}
	else if(isHovered()){
		drawHoverBox();
	}
}

void ofxMidiMap::ValueDisplay::update(){
}

void ofxMidiMap::ValueDisplay::setSelected(bool selected){
	this->selected = selected;
}

void ofxMidiMap::ValueDisplay::setValue(string value){
	this->value = value;
}

bool ofxMidiMap::ValueDisplay::inBounds(int x, int y){
	return (x >= bounds.x && x <= bounds.x+bounds.width && y >= bounds.y && y <= bounds.y+bounds.height);
}

bool ofxMidiMap::ValueDisplay::isHovered(){
	return this->hover;
}

bool ofxMidiMap::ValueDisplay::isSelected(){
	return this->selected;
}

bool ofxMidiMap::ValueDisplay::mouseMoved(int x, int y){
	if(inBounds(x, y))
		hover = true;
	else if(hover)
		hover = false;
	return hover;
}

bool ofxMidiMap::ValueDisplay::mouseDragged(int x, int y, int button){
	bool result = false;
	return result;
}

bool ofxMidiMap::ValueDisplay::mousePressed(int x, int y, int button){
	bool result = false;
	if(inBounds(x, y)){
		this->setSelected(!selected);
		result = true;
	}
	return result;
}

bool ofxMidiMap::ValueDisplay::mouseReleased(int x, int y, int button){
	bool result = false;
	return result;
}

/*** ValueDisplayGroup ***/
ofxMidiMap::ValueDisplayGroup::ValueDisplayGroup(string name){
	setName(name);
}

ofxMidiMap::ValueDisplayGroup::~ValueDisplayGroup(){
	for(map<string,ValueDisplay*>::iterator vdIt = values.begin(); vdIt != values.end(); vdIt++){
		if(values[vdIt->first] != NULL){
			delete values[vdIt->first];
			values[vdIt->first] = NULL;
		}
	}
	values.clear();
}

void ofxMidiMap::ValueDisplayGroup::draw(float x, float y){
	float maxH = ofGetHeight()-30.0;
	float spacing = 5.0;
	
	float minX = spacing;
	float minY = spacing;
	float cX = minX;
	float cY = minY;
	
	float width = 0.0;
	float height = 0.0;
	
	float cWidth = 0.0;
	float cHeight = 0.0;
	if(name != ""){
		ofDrawBitmapString(name, x+cX, y+20.0+cY);
		cWidth = name.length()*10.0;
		minY += 25.0+spacing;
		cY = minY;
		if(cX+cWidth> width)
			width = cX+cWidth;
		if(cY > height)
			height = cY;
	}
	
	float cColWidth = 0.0;
	for(map<string,ValueDisplay*>::iterator vdIt = values.begin(); vdIt != values.end(); vdIt++){
		ValueDisplay* cValue = vdIt->second;
		if(cValue != NULL){
			cValue->draw(x+cX, y+cY);
			
			cWidth = cValue->bounds.width;
			cHeight = cValue->bounds.height;
			
			if(cWidth+spacing > cColWidth)
				cColWidth = cWidth+spacing;
			
			if(cX+cColWidth > width)
				width = cX+cColWidth;
			
			cY += cHeight+spacing;
			
			if(cY > height)
				height = cY;
			
			if(cY > maxH-cHeight-spacing){
				cY = minY;
				cX += cColWidth;
				cColWidth = 0.0;
			}
		}
	}
	
	ofNoFill();
	ofRect(x, y, width, height);
}

void ofxMidiMap::ValueDisplayGroup::update(){
	for(map<string,ValueDisplay*>::iterator vdIt = values.begin(); vdIt != values.end(); vdIt++){
		ValueDisplay* cValue = vdIt->second;
		if(cValue != NULL)
			cValue->update();
	}
}

ofxMidiMap::ValueDisplay* ofxMidiMap::ValueDisplayGroup::addValue(string name, ValueDisplay* value){
	ValueDisplay* result = NULL;
	if(values.count(name) > 0 && values[name] != value)
		result = values[name]; // we return the old value object if we are replacing it...
	values[name] = value;
	return result;
}

void ofxMidiMap::ValueDisplayGroup::setName(string name){
	this->name = name;
}

void ofxMidiMap::ValueDisplayGroup::setValue(string name, string value){
	if(values.count(name) <= 0)
		values[name] = new ofxMidiMap::ValueDisplay(name, value);
	else
		values[name]->setValue(value);
}

bool ofxMidiMap::ValueDisplayGroup::mouseMoved(int x, int y){
	bool result = false;
	for(map<string,ValueDisplay*>::iterator vdIt = values.begin(); vdIt != values.end(); vdIt++){
		ValueDisplay* cValue = vdIt->second;
		if(cValue != NULL){
			if(cValue->mouseMoved(x, y))
				result = true;
		}
	}
	return result;
}

bool ofxMidiMap::ValueDisplayGroup::mouseDragged(int x, int y, int button){
	bool result = false;
	for(map<string,ValueDisplay*>::iterator vdIt = values.begin(); vdIt != values.end(); vdIt++){
		ValueDisplay* cValue = vdIt->second;
		if(cValue != NULL){
			if(cValue->mouseDragged(x, y, button))
				result = true;
		}
	}
	return result;
}

bool ofxMidiMap::ValueDisplayGroup::mousePressed(int x, int y, int button){
	bool result = false;
	for(map<string,ValueDisplay*>::iterator vdIt = values.begin(); vdIt != values.end(); vdIt++){
		ValueDisplay* cValue = vdIt->second;
		if(cValue != NULL){
			if(cValue->mousePressed(x, y, button))
				result = true;
		}
	}
	return result;
}

bool ofxMidiMap::ValueDisplayGroup::mouseReleased(int x, int y, int button){
	bool result = false;
	for(map<string,ValueDisplay*>::iterator vdIt = values.begin(); vdIt != values.end(); vdIt++){
		ValueDisplay* cValue = vdIt->second;
		if(cValue != NULL){
			if(cValue->mouseReleased(x, y, button))
				result = true;
		}
	}
	return result;
}
