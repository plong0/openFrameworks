/*
 *  ofxLiteGroup.cpp
 *  emptyExample
 *
 *  Created by Eli Smiles on 11-04-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxLiteGroup.h"

ofxLiteGroup::ofxLiteGroup(string name){
	setName(name);
	setSpacing();
	setMaxHeight();
	setSortable(false, false);
	dragBox = NULL;
}

ofxLiteGroup::~ofxLiteGroup(){
	for(int i=0; i < sortedBoxes.size(); i++)
		sortedBoxes[i] = NULL;
	sortedBoxes.clear();
	
	for(map<string,ofxLiteBox*>::iterator boxIt = boxes.begin(); boxIt != boxes.end(); boxIt++){
		if(boxes[boxIt->first] != NULL){
			delete boxes[boxIt->first];
			boxes[boxIt->first] = NULL;
		}
	}
	boxes.clear();
}


void ofxLiteGroup::draw(float x, float y){
	if(maxHeight == -1.0) maxHeight = ofGetHeight() - y;
	
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
//	for(map<string,ofxLiteBox*>::iterator boxIt = boxes.begin(); boxIt != boxes.end(); boxIt++){
//		ofxLiteBox* cBox = boxIt->second;
	for(int i=0; i < sortedBoxes.size(); i++){
		ofxLiteBox* cBox = sortedBoxes[i];
		if(cBox != NULL){
			cBox->draw(x+cX, y+cY);
			
			ofRectangle cBounds = cBox->getBounds();
			
			cWidth = cBounds.width;
			cHeight = cBounds.height;
			
			if(cWidth+spacing > cColWidth)
				cColWidth = cWidth+spacing;
			
			if(cX+cColWidth > width)
				width = cX+cColWidth;
			
			cY += cHeight+spacing;
			
			if(cY > height)
				height = cY;
			
			if(cY > maxHeight-cHeight-spacing){
				cY = minY;
				cX += cColWidth;
				cColWidth = 0.0;
			}
		}
	}
	
	ofNoFill();
	ofRect(x, y, width, height);
}

void ofxLiteGroup::update(){
	for(map<string,ofxLiteBox*>::iterator boxIt = boxes.begin(); boxIt != boxes.end(); boxIt++){
		ofxLiteBox* cBox = boxIt->second;
		if(cBox != NULL){
			cBox->update();
		}
	}
}

int ofxLiteGroup::getPosition(float x, float y){
	int result = -1;
	int count = 0;
	for(int i=0; i < sortedBoxes.size(); i++){
		ofRectangle cBounds = sortedBoxes[i]->getBounds();

		if(x >= cBounds.x && x <= cBounds.x+cBounds.width+spacing && y >= cBounds.y-spacing && y <= cBounds.y+cBounds.height+spacing){
			result = i;
		}
	}
//	result = sortedBoxes.size()-1;
	return result;
}

void ofxLiteGroup::sortBox(ofxLiteBox* box, int position){
	if(position >= 0 && position < sortedBoxes.size()){
		removeBoxFromSorted(box);
		sortedBoxes.insert(sortedBoxes.begin()+position, box);
	}
}

void ofxLiteGroup::removeBoxFromSorted(ofxLiteBox* box){
	for(int i=0; i < sortedBoxes.size(); i++){
		if(sortedBoxes[i] == box){
			sortedBoxes.erase(sortedBoxes.begin()+i);
			break;
		}
	}
}

ofxLiteBox* ofxLiteGroup::addBox(string name, ofxLiteBox* value){
	ofxLiteBox* result = NULL;
	if(boxes.count(name) > 0 && boxes[name] != value)
		result = boxes[name]; // we return the old value object if we are replacing it...
	boxes[name] = value;
	sortedBoxes.push_back(value);
	if(autoBoxDrags && boxes[name] != NULL)
		boxes[name]->setDraggable(sortable);
	if(result != NULL)
		removeBoxFromSorted(result);
	return result;
}

map<string,ofxLiteBox*>* ofxLiteGroup::getBoxes(){
	return &boxes;
}

bool ofxLiteGroup::hasBox(string name){
	return (boxes.count(name) > 0);
}

void ofxLiteGroup::setMaxHeight(float maxHeight){
	this->maxHeight = maxHeight;
}

void ofxLiteGroup::setName(string name){
	this->name = name;
}

void ofxLiteGroup::setSortable(bool sortable, bool updateBoxDrags){
	this->sortable = sortable;
	this->autoBoxDrags = updateBoxDrags;
	if(autoBoxDrags){
		for(map<string,ofxLiteBox*>::iterator boxIt = boxes.begin(); boxIt != boxes.end(); boxIt++){
			ofxLiteBox* cBox = boxIt->second;
			cBox->setDraggable(this->sortable);
		}
	}
}

void ofxLiteGroup::setSpacing(float spacing){
	this->spacing = spacing;
}

ofxLiteBox* ofxLiteGroup::setValue(string name, string value){
	if(boxes.count(name) <= 0)
		addBox(name, new ofxLiteBox(name, value));
	else
		boxes[name]->setValue(value);
	return boxes[name];
}

bool ofxLiteGroup::mouseMoved(int x, int y){
	bool result = false;
	for(map<string,ofxLiteBox*>::iterator boxIt = boxes.begin(); boxIt != boxes.end(); boxIt++){
		ofxLiteBox* cBox = boxIt->second;
		if(cBox != NULL){
			if(cBox->mouseMoved(x, y))
				result = true;
		}
	}
	return result;
}

bool ofxLiteGroup::mouseDragged(int x, int y, int button){
	bool result = false;
	for(map<string,ofxLiteBox*>::iterator boxIt = boxes.begin(); boxIt != boxes.end(); boxIt++){
		ofxLiteBox* cBox = boxIt->second;
		if(cBox != NULL){
			if((dragBox == NULL || dragBox == cBox) && cBox->mouseDragged(x, y, button)){
				dragBox = cBox;
				dragBox->setSelected(false);
				result = true;
			}
		}
	}
	if(sortable && dragBox != NULL){
		int mouseIdx = getPosition(x, y);
		if(mouseIdx != -1){
			sortBox(dragBox, mouseIdx);
		}
	}
	return result;
}

bool ofxLiteGroup::mousePressed(int x, int y, int button){
	bool result = false;
	for(map<string,ofxLiteBox*>::iterator boxIt = boxes.begin(); boxIt != boxes.end(); boxIt++){
		ofxLiteBox* cBox = boxIt->second;
		if(cBox != NULL){
			if(cBox->mousePressed(x, y, button))
				result = true;
		}
	}
	return result;
}

bool ofxLiteGroup::mouseReleased(int x, int y, int button){
	bool result = false;
	dragBox = NULL;
	for(map<string,ofxLiteBox*>::iterator boxIt = boxes.begin(); boxIt != boxes.end(); boxIt++){
		ofxLiteBox* cBox = boxIt->second;
		if(cBox != NULL){
			if(cBox->mouseReleased(x, y, button))
				result = true;
		}
	}
	return result;
}

