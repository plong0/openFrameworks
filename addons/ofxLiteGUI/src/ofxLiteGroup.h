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

class ofxLiteEventGroupSorted : public ofxLiteEvent{
public:
	vector<ofxLiteBox*>* sortedBoxes;
	int oldPosition, newPosition;
	
	ofxLiteEventGroupSorted(ofxLiteBox* lite=NULL, vector<ofxLiteBox*>* sortedBoxes=NULL):ofxLiteEvent(lite){
		this->sortedBoxes = sortedBoxes;
		oldPosition = newPosition = -1;
	};
};

class ofxLiteGroup : public ofxLiteBox{
protected:
	map<string,ofxLiteBox*> boxes;
	float spacing;
	float maxHeight;
	
	bool sortable, autoBoxDrags;
	vector<ofxLiteBox*> sortedBoxes;
	
	ofPoint mouse;
	ofxLiteBox* dragBox;
	
	virtual void onSort(int oldPosition=-1, int newPosition=-1);
	
	int getPosition(float x, float y);
	int getPosition(ofxLiteBox* box);
	void removeBoxFromSorted(ofxLiteBox* box);
	void sortBox(ofxLiteBox* box, int position);
	
public:
	ofxLiteGroup(string name="LiteGroup");
	~ofxLiteGroup();
	
	virtual void drawContent(float x=-1.0, float y=-1.0);
	void update();
	
	ofxLiteBox* addBox(string name, ofxLiteBox* value);
	virtual void listenToBox(ofxLiteBox* box);
	
	map<string,ofxLiteBox*>* getBoxes();
	vector<ofxLiteBox*>* getSortedBoxes();
	
	bool hasBox(string name);
	ofxLiteBox* removeBox(string name, bool doDelete=true);
	
	void setMaxHeight(float maxHeight=-1.0);
	void setSortable(bool sortable=false, bool updateBoxDrags=true);
	void setSpacing(float spacing=5.0);
	ofxLiteBox* setValue(string name, string value);
	
	virtual bool mouseMoved(int x, int y);
	virtual bool mouseDragged(int x, int y, int button);
	virtual bool mousePressed(int x, int y, int button);
	virtual bool mouseReleased(int x, int y, int button);
	
	void childBoxDragged(ofxLiteEventBoxDragged& event);
	void childBoxDragHovered(ofxLiteEventBoxDragHovered& event);
	void childBoxHovered(ofxLiteEventBoxHovered& event);
	void childBoxPressed(ofxLiteEventBoxPressed& event);
	void childBoxSelected(ofxLiteEventBoxSelected& event);
	void childBoxTriggered(ofxLiteEventBoxTriggered& event);
	
	struct{
		ofEvent<ofxLiteEventGroupSorted> groupSorted;
	} groupEvents;
};

#endif
