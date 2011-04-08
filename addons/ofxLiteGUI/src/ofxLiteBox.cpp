/*
 *  ofxLiteBox.cpp
 *  emptyExample
 *
 *  Created by Eli Smiles on 11-04-07.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxLiteBox.h"

ofxLiteBox::ofxLiteBox(string name, string value){
	setName(name);
	setValue(value);
	setBounds();
	setPadding(5.0, 5.0, 5.0, 5.0);
	
	draggable = false;
	pressOffset.x = pressOffset.y = -1.0;
	dragPosition.x = dragPosition.y = 0.0;
	
	hover = false;
	selected = false;
	dragging = false;
}

ofxLiteBox::~ofxLiteBox(){
}

void ofxLiteBox::draw(float x, float y){
	if(dynamicBounds){
		if(bounds.x != x) bounds.x = x;
		if(bounds.y != y) bounds.y = y;
	}
	
	string output = getOutputValue();
	
	float width = getTextWidth(output) + padTL.x + padBR.x;
	float height = getTextHeight(output) + padTL.y + padBR.y;
	
	if(dynamicBounds){
		if(bounds.width != width) bounds.width = width;
		if(bounds.height != height) bounds.height = height;
	}
	
	drawContent();
	
	if(isSelected()){
		drawSelectedBox();
	}
	else if(isHovered()){
		drawHoverBox();
	}
	
	if(draggable && dragging){
		drawContent(dragPosition.x-pressOffset.x, dragPosition.y-pressOffset.y); //bounds.x+dragOffset.x, bounds.y+dragOffset.y);
	}
}

void ofxLiteBox::drawContent(float x, float y){
	if(x == -1.0) x = bounds.x;
	if(y == -1.0) y = bounds.y;
	
	ofDrawBitmapString(getOutputValue(), x+padTL.x, y+15.0+padTL.y);
	
	ofNoFill();
	ofRect(x, y, bounds.width, bounds.height);
}

void ofxLiteBox::drawHoverBox(float x, float y){
	if(x == -1.0) x = bounds.x;
	if(y == -1.0) y = bounds.y;
	
	ofNoFill();
	ofRect(x+2, y+2, bounds.width-4, bounds.height-4);
}

void ofxLiteBox::drawSelectedBox(float x, float y){
	if(x == -1.0) x = bounds.x;
	if(y == -1.0) y = bounds.y;
	
	ofSetLineWidth(3.0);
	ofNoFill();
	ofRect(x+1, y+1, bounds.width-2, bounds.height-2);
	ofSetLineWidth(1.0);
}

void ofxLiteBox::update(){
}

ofRectangle ofxLiteBox::getBounds(){
	return bounds;
}

string ofxLiteBox::getName(){
	return name;
}

string ofxLiteBox::getOutputValue(){
	stringstream output;
	output << getName();
	if(getValue() != "") output << " [" << getValue() << "]";
	return output.str();
}

float ofxLiteBox::getTextWidth(string text){
	return text.length()*8.5;
}

float ofxLiteBox::getTextHeight(string text){
	return 15.0;
}

string ofxLiteBox::getValue(){
	return value;
}

bool ofxLiteBox::isDragging(){
	return this->dragging;
}

bool ofxLiteBox::isHovered(){
	return this->hover;
}

bool ofxLiteBox::isSelected(){
	return this->selected;
}

void ofxLiteBox::setBounds(float x, float y, float w, float h, bool dynamicBounds){
	bounds.x = x;
	bounds.y = y;
	bounds.width = w;
	bounds.height = h;
	this->dynamicBounds = dynamicBounds;
}

void ofxLiteBox::setDraggable(bool draggable){
	this->draggable = draggable;
}

void ofxLiteBox::setName(string name){
	this->name = name;
}

void ofxLiteBox::setValue(string value){
	this->value = value;
}

void ofxLiteBox::setPadding(float top, float left, float bottom, float right){
	padTL.y = top;
	padTL.x = left;
	padBR.y = bottom;
	padBR.x = right;
}

void ofxLiteBox::setSelected(bool selected){
	this->selected = selected;
}

bool ofxLiteBox::inBounds(int x, int y){
	return (x >= bounds.x && x <= bounds.x+bounds.width && y >= bounds.y && y <= bounds.y+bounds.height);
}

bool ofxLiteBox::mouseMoved(int x, int y){
	if(inBounds(x, y))
		hover = true;
	else if(hover)
		hover = false;
	return hover;
}

bool ofxLiteBox::mouseDragged(int x, int y, int button){
	bool result = false;
	if(draggable){
		if(pressOffset.x != -1.0 && pressOffset.y != -1.0){
			dragging = true;
			result = true;
		}
		
		dragPosition.x = x;
		dragPosition.y = y;
	}
	return result;
}

bool ofxLiteBox::mousePressed(int x, int y, int button){
	bool result = false;
	if(inBounds(x, y)){
		pressOffset.x = x-bounds.x;
		pressOffset.y = y-bounds.y;
		this->setSelected(!selected);
		result = true;
	}
	return result;
}

bool ofxLiteBox::mouseReleased(int x, int y, int button){
	bool result = false;
	pressOffset.x = pressOffset.y = -1.0;
	dragPosition.x = dragPosition.y = 0.0;
	dragging = false;
	return result;
}
