/*
 *  ofxLiteBox.h
 *  emptyExample
 *
 *  Created by Eli Smiles on 11-04-07.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _OFX_LITE_GUI_BOX
#define _OFX_LITE_GUI_BOX

#include "ofMain.h"
#include "ofxLiteBoxTypes.h"
#include "ofxLiteEvent.h"

class ofxLiteEventBoxDragged : public ofxLiteEvent{
public:
	ofPoint draggedTo;
	bool isDragging;
	
	ofxLiteEventBoxDragged(ofxLiteBox* lite=NULL, bool isDragging=false, ofPoint draggedTo=ofPoint(-1.0,-1.0)):ofxLiteEvent(lite){
		this->draggedTo = draggedTo;
		this->isDragging = isDragging;
	};
};

class ofxLiteEventBoxDragHovered : public ofxLiteEvent{
public:
	bool isHovered;
	ofPoint hoveredAt;
	
	ofxLiteEventBoxDragHovered(ofxLiteBox* lite=NULL, bool isHovered=true, ofPoint hoveredAt=ofPoint(-1.0,-1.0)):ofxLiteEvent(lite){
		this->isHovered = isHovered;
		this->hoveredAt = hoveredAt;
	};
};

class ofxLiteEventBoxHovered : public ofxLiteEvent{
public:
	bool isHovered;
	ofPoint hoveredAt;
	
	ofxLiteEventBoxHovered(ofxLiteBox* lite=NULL, bool isHovered=true, ofPoint hoveredAt=ofPoint(-1.0,-1.0)):ofxLiteEvent(lite){
		this->isHovered = isHovered;
		this->hoveredAt = hoveredAt;
	};
};

class ofxLiteEventBoxPressed : public ofxLiteEvent{
public:
	bool isPressed;
	ofPoint pressedAt;

	ofxLiteEventBoxPressed(ofxLiteBox* lite=NULL, bool isPressed=true, ofPoint pressedAt=ofPoint(-1.0,-1.0)):ofxLiteEvent(lite){
		this->isPressed = isPressed;
		this->pressedAt = pressedAt;
	};
};

class ofxLiteEventBoxSelected : public ofxLiteEvent{
public:
	bool isSelected;
	
	ofxLiteEventBoxSelected(ofxLiteBox* lite=NULL, bool isSelected=true):ofxLiteEvent(lite){
		this->isSelected = isSelected;
	};
};

class ofxLiteEventBoxTriggered : public ofxLiteEvent{
public:	
	ofxLiteEventBoxTriggered(ofxLiteBox* lite=NULL):ofxLiteEvent(lite){
	};
};

class ofxLiteBox {
protected:
	string name;
	string value;
	ofRectangle bounds;
	bool dynamicBounds;
	ofPoint padTL;
	ofPoint padBR;
	
	ofPoint pressOffset;
	ofPoint dragPosition;
	
	bool draggable;
	bool dragOffRelease;
	bool dragOnPress;
	bool hoverable;
	bool selectable;
	bool triggerable;
	bool triggerWhenSelected;
	
	bool dragging;
	bool hovered;
	bool mouseOn;
	bool pressed;
	bool selected;
	
	int lastTrigger;
	int triggerDelay;
	
	virtual void onDrag(bool dragging, ofPoint dragTo);
	virtual void onDragOn(ofPoint draggedAt);
	virtual void onDragOff(ofPoint draggedAt);
	virtual void onHover(bool hovered, ofPoint hoverAt);
	virtual void onPress(ofPoint pressAt);
	virtual void onRelease(ofPoint releaseAt);
	virtual void onSelect(bool selected);
	virtual void onTrigger();
	
	float getTextWidth(string text);
	float getTextHeight(string text);
	
	bool inBounds(int x, int y);
	
	
public:
	ofxLiteBox(string name="LiteBox", string value="");
	~ofxLiteBox();
	
	virtual int getBoxType(){ return OFX_LITE_BOX_TYPE_BOX; };
	
	void configureAsButton(int triggerDelay=150);
	void configureAsDraggableBox();
	void configureAsToggle();
	
	virtual void draw(float x, float y);
	virtual void drawContent(float x=-1.0, float y=-1.0);
	void drawHoverBox(float x=-1.0, float y=-1.0);
	void drawSelectedBox(float x=-1.0, float y=-1.0);
	
	virtual void update();
	
	ofRectangle getBounds();
	string getName();
	virtual string getOutputValue();
	string getValue();
	
	bool isDragging();
	bool isHovered();
	bool isPressed();
	bool isSelected();
	bool isTriggered();
	
	void setDraggable(bool draggable);
	void setDragOffRelease(bool dragOffRelease);
	void setDragOnPress(bool dragOnPress);
	void setHoverable(bool hoverable);
	void setSelectable(bool selectable);
	void setTriggerable(bool triggerable, int triggerDelay=150, bool whenSelected=true);
	
	void setBounds(float x=0.0, float y=0.0, float w=0.0, float h=0.0, bool dynamicBounds=true);
	void setName(string name);
	void setPadding(float top=0.0, float left=0.0, float bottom=0.0, float right=0.0);
	
	virtual void setDragging(bool dragging, ofPoint dragAt=ofPoint(-1.0,-1.0));
	virtual void setHovered(bool hovered, ofPoint hoverAt=ofPoint(-1.0,-1.0));
	virtual void setPressed(bool pressed, ofPoint pressAt=ofPoint(-1.0,-1.0), bool updatePressOffset=true);
	virtual void setSelected(bool selected);
	virtual void setValue(string value);
	
	virtual bool mouseMoved(int x, int y);
	virtual bool mouseDragged(int x, int y, int button);
	virtual bool mousePressed(int x, int y, int button);
	virtual bool mouseReleased(int x, int y, int button);
	
	struct{
		ofEvent<ofxLiteEventBoxDragged> boxDragged;
		ofEvent<ofxLiteEventBoxDragHovered> boxDragHovered;
		ofEvent<ofxLiteEventBoxHovered> boxHovered;
		ofEvent<ofxLiteEventBoxPressed> boxPressed;
		ofEvent<ofxLiteEventBoxSelected> boxSelected;
		ofEvent<ofxLiteEventBoxTriggered> boxTriggered;
	} boxEvents;
};

#endif
