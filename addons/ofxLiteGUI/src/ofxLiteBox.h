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
	
	bool hover;
	bool selected;
	bool dragging;
	
	float getTextWidth(string text);
	float getTextHeight(string text);
	
	bool inBounds(int x, int y);
	
	
public:
	ofxLiteBox(string name="LiteBox", string value="");
	~ofxLiteBox();
	
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
	bool isSelected();
	
	void setBounds(float x=0.0, float y=0.0, float w=0.0, float h=0.0, bool dynamicBounds=true);
	void setDraggable(bool draggable);
	void setName(string name);
	void setPadding(float top=0.0, float left=0.0, float bottom=0.0, float right=0.0);
	virtual void setSelected(bool selected);
	virtual void setValue(string value);
	
	virtual bool mouseMoved(int x, int y);
	virtual bool mouseDragged(int x, int y, int button);
	virtual bool mousePressed(int x, int y, int button);
	virtual bool mouseReleased(int x, int y, int button);

};

#endif
