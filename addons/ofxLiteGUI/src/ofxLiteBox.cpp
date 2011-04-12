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
	
	pressOffset.x = pressOffset.y = -1.0;
	dragPosition.x = dragPosition.y = 0.0;
	
	draggable = false;
	dragOffRelease = false;
	dragOnPress = false;
	hoverable = true;
	selectable = false;
	triggerable = false;
	triggerWhenSelected = true;
	
	dragging = false;
	hovered = false;
	mouseOn = false;
	pressed = false;
	selected = false;
	triggerDelay = lastTrigger = -1;
	
	configureAsDraggableBox();
}

ofxLiteBox::~ofxLiteBox(){
}

void ofxLiteBox::configureAsButton(int triggerDelay){
	setDraggable(false);
	setDragOnPress(true);
	setDragOffRelease(true);
	setSelectable(false);
	setTriggerable(true, triggerDelay);
}

void ofxLiteBox::configureAsDraggableBox(){
	setDraggable(true);
	setDragOnPress(false);
	setDragOffRelease(false);
	setSelectable(false);
	setTriggerable(false);
}

void ofxLiteBox::configureAsToggle(){
	setDraggable(false);
	setDragOnPress(true);
	setDragOffRelease(true);
	setSelectable(true);
	setTriggerable(false);
}

void ofxLiteBox::onDrag(bool dragging, ofPoint dragTo){
	ofxLiteEventBoxDragged eventArgs(this, dragging, dragTo);
	ofNotifyEvent(boxEvents.boxDragged, eventArgs, this);
}

void ofxLiteBox::onDragOn(ofPoint draggedAt){
	ofxLiteEventBoxDragHovered eventArgs(this, true, draggedAt);
	ofNotifyEvent(boxEvents.boxDragHovered, eventArgs, this);
	
	if(dragOnPress && !pressed)
		setPressed(true, draggedAt, !isDragging());
	
	if(hoverable)
		setHovered(true, draggedAt);
}

void ofxLiteBox::onDragOff(ofPoint draggedAt){
	ofxLiteEventBoxDragHovered eventArgs(this, false, draggedAt);
	ofNotifyEvent(boxEvents.boxDragHovered, eventArgs, this);
	
	if(dragOffRelease)
		setPressed(false, draggedAt, false);
	
	if(hoverable)
		setHovered(false, draggedAt);
}

void ofxLiteBox::onHover(bool hovered, ofPoint hoverAt){
	ofxLiteEventBoxHovered eventArgs(this, hovered, hoverAt);
	ofNotifyEvent(boxEvents.boxHovered, eventArgs, this);
}

void ofxLiteBox::onPress(ofPoint pressAt){
	ofxLiteEventBoxPressed eventArgs(this, true, pressAt);
	ofNotifyEvent(boxEvents.boxPressed, eventArgs, this);
	
	if(selectable)
		this->setSelected(!isSelected());
}

void ofxLiteBox::onRelease(ofPoint releaseAt){
	ofxLiteEventBoxPressed eventArgs(this, false, releaseAt);
	ofNotifyEvent(boxEvents.boxPressed, eventArgs, this);
	
}

void ofxLiteBox::onSelect(bool selected){
	ofxLiteEventBoxSelected eventArgs(this, selected);
	ofNotifyEvent(boxEvents.boxSelected, eventArgs, this);
}

void ofxLiteBox::onTrigger(){
	ofxLiteEventBoxTriggered eventArgs(this);
	ofNotifyEvent(boxEvents.boxTriggered, eventArgs, this);
}

void ofxLiteBox::draw(float x, float y){
	if(dynamicBounds){
		if(bounds.x != x) bounds.x = x;
		if(bounds.y != y) bounds.y = y;
	}
	
	drawContent();
	
	if(isSelected() || isTriggered()){
		drawSelectedBox();
	}
	else if(isHovered() || isDragging()){
		drawHoverBox();
	}
	
	if(draggable && dragging){
		bool oldDynBounds = dynamicBounds;
		dynamicBounds = false;
		drawContent(dragPosition.x-pressOffset.x, dragPosition.y-pressOffset.y); //bounds.x+dragOffset.x, bounds.y+dragOffset.y);
		dynamicBounds = oldDynBounds;
	}
}

void ofxLiteBox::drawContent(float x, float y){
	if(x == -1.0) x = bounds.x;
	if(y == -1.0) y = bounds.y;
	
	string output = getOutputValue();
	
	float width = getTextWidth(output) + padTL.x + padBR.x;
	float height = getTextHeight(output) + padTL.y + padBR.y;
	
	if(dynamicBounds){
		if(bounds.width != width) bounds.width = width;
		if(bounds.height != height) bounds.height = height;
	}
	
	ofDrawBitmapString(output, x+padTL.x, y+15.0+padTL.y);
	
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
	if(isTriggered()){
		int cTime = ofGetElapsedTimeMillis();
		
		if(lastTrigger == -1 || (triggerDelay != -1 && (cTime-lastTrigger) > triggerDelay)){
			onTrigger();
			lastTrigger = cTime;
		}
	}
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
	return this->hovered;
}

bool ofxLiteBox::isPressed(){
	return this->pressed;
}

bool ofxLiteBox::isSelected(){
	return this->selected;
}

bool ofxLiteBox::isTriggered(){
	return (this->triggerable && this->isPressed() && (!selectable || !triggerWhenSelected || isSelected()));
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

void ofxLiteBox::setDragOffRelease(bool dragOffRelease){
	this->dragOffRelease = dragOffRelease;
}

void ofxLiteBox::setDragOnPress(bool dragOnPress){
	this->dragOnPress = dragOnPress;
}

void ofxLiteBox::setHoverable(bool hoverable){
	this->hoverable = hoverable;
}

void ofxLiteBox::setSelectable(bool selectable){
	this->selectable = selectable;
}

void ofxLiteBox::setTriggerable(bool triggerable, int triggerDelay, bool whenSelected){
	this->triggerable = triggerable;
	this->triggerDelay = triggerDelay;
	this->triggerWhenSelected = whenSelected;
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

void ofxLiteBox::setDragging(bool dragging, ofPoint dragAt){
	if(pressOffset.x != -1.0 && pressOffset.y != -1.0){
		if(dragging || this->dragging)
			onDrag(dragging, dragAt);
		this->dragging = dragging;
		
		if(!dragging)
			dragPosition.x = dragPosition.y = 0.0;
	}
}

void ofxLiteBox::setHovered(bool hovered, ofPoint hoverAt){
	if(this->hovered != hovered){
		this->hovered = hovered;
		
		if(hoverAt.x != -1.0 && hoverAt.y != -1.0)
			onHover(hovered, hoverAt);
	}
}

void ofxLiteBox::setPressed(bool pressed, ofPoint pressAt, bool updatePressOffset){
	if(updatePressOffset){
		if(pressed){
			pressOffset.x = pressAt.x-bounds.x;
			pressOffset.y = pressAt.y-bounds.y;
		}
		else{
			pressOffset.x = -1.0;
			pressOffset.y = -1.0;
		}
	}
	
	if(this->pressed != pressed){
		this->pressed = pressed;
		
		if(pressed){
			lastTrigger = -1;
			onPress(pressAt);
		}
		else
			onRelease(pressAt);
	}
}

void ofxLiteBox::setSelected(bool selected){
	if(this->selected != selected){
		this->selected = selected;

		onSelect(selected);
	}
}

bool ofxLiteBox::inBounds(int x, int y){
	return (x >= bounds.x && x <= bounds.x+bounds.width && y >= bounds.y && y <= bounds.y+bounds.height);
}

bool ofxLiteBox::mouseMoved(int x, int y){
	mouseOn = inBounds(x, y);
	if(hoverable){
		if(mouseOn)
			setHovered(true, ofPoint(x, y));
		else
			setHovered(false, ofPoint(x, y));
	}
	return isHovered();
}

bool ofxLiteBox::mouseDragged(int x, int y, int button){
	bool result = false;
	
	bool wasMouseOn = mouseOn;
	mouseOn = inBounds(x, y);
	
	if(!wasMouseOn && mouseOn)
		onDragOn(ofPoint(x, y));
	if(wasMouseOn && !mouseOn)
		onDragOff(ofPoint(x, y));
	
	if(draggable){
		dragPosition.x = x;
		dragPosition.y = y;
		
		setDragging(true, dragPosition);
		result = dragging;
	}
	return result;
}

bool ofxLiteBox::mousePressed(int x, int y, int button){
	bool result = false;
	mouseOn = inBounds(x, y);
	if(mouseOn){
		setPressed(true, ofPoint(x, y));
		result = true;
	}
	return result;
}

bool ofxLiteBox::mouseReleased(int x, int y, int button){
	bool result = false;
	mouseOn = inBounds(x, y);
	if(mouseOn)
		result = true;
	setDragging(false, ofPoint(x, y));
	setPressed(false, ofPoint(x, y));
	return result;
}
