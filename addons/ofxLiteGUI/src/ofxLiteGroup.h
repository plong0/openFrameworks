/*
 *  ofxLiteGroup.h
 *  emptyExample
 *
 *  Created by Eli Smiles on 11-04-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _OFX_LITE_GUI_GROUP
#define _OFX_LITE_GUI_GROUP

#include "ofxLiteBox.h"

class ofxLiteGroup{
protected:
	string name;
	map<string,ofxLiteBox*> boxes;
	float spacing;
	float maxHeight;
	
	bool sortable, autoBoxDrags;
	vector<ofxLiteBox*> sortedBoxes;
	
	ofPoint mouse;
	ofxLiteBox* dragBox;
	
	int getPosition(float x, float y);
	
	void removeBoxFromSorted(ofxLiteBox* box);
	void sortBox(ofxLiteBox* box, int position);
	
public:
	ofxLiteGroup(string name="LiteGroup");
	~ofxLiteGroup();
	
	void draw(float x, float y);
	void update();
	
	ofxLiteBox* addBox(string name, ofxLiteBox* value);
	
	map<string,ofxLiteBox*>* getBoxes();
	vector<ofxLiteBox*> getSortedBoxes();
	
	bool hasBox(string name);
	
	void setMaxHeight(float maxHeight=-1.0);
	void setName(string name);
	void setSortable(bool sortable=false, bool updateBoxDrags=true);
	void setSpacing(float spacing=5.0);
	ofxLiteBox* setValue(string name, string value);
	
	bool mouseMoved(int x, int y);
	bool mouseDragged(int x, int y, int button);
	bool mousePressed(int x, int y, int button);
	bool mouseReleased(int x, int y, int button);
};

#endif
