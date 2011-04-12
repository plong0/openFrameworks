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

class ofxLiteEventGroupSingleSelected : public ofxLiteEvent{
public:
	ofxLiteBox* selected;
	ofxLiteEventGroupSingleSelected(ofxLiteBox* lite=NULL, ofxLiteBox* selected=NULL):ofxLiteEvent(lite){
		this->selected = selected;
	};
};

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
	
	ofPoint mouse;
	ofxLiteBox* dragBox;
	
	bool sortable, autoBoxDrags;
	vector<ofxLiteBox*> sortedBoxes;
	
	bool singleSelect, autoSetToggles;
	ofxLiteBox* singleSelected;
	
	virtual void onSingleSelect(ofxLiteBox* singleSelected);
	virtual void onSort(int oldPosition=-1, int newPosition=-1);
	
	int getPosition(float x, float y);
	int getPosition(ofxLiteBox* box);
	void removeBoxFromSorted(ofxLiteBox* box);
	void sortBox(ofxLiteBox* box, int position);
	
public:
	ofxLiteGroup(string name="LiteGroup");
	~ofxLiteGroup();
	
	virtual int getBoxType(){ return OFX_LITE_BOX_TYPE_GROUP; };
	
	bool configureAsRadioSet(bool autoSetToggles=true);
	
	virtual void drawContent(float x=-1.0, float y=-1.0);
	void update();
	
	ofxLiteBox* addBox(string name, ofxLiteBox* value);
	
	map<string,ofxLiteBox*>* getBoxes();
	vector<ofxLiteBox*>* getSortedBoxes();
	
	void setSingleSelect(bool singleSelect, bool autoSetToggles=true, string selectName="");
	void setSingleSelected(ofxLiteBox* select);
	
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
	
	virtual void listenToBox(ofxLiteBox* box);
	virtual void listenToGroup(ofxLiteGroup* group);
	
	void childBoxDragged(ofxLiteEventBoxDragged& event);
	void childBoxDragHovered(ofxLiteEventBoxDragHovered& event);
	void childBoxHovered(ofxLiteEventBoxHovered& event);
	void childBoxPressed(ofxLiteEventBoxPressed& event);
	void childBoxSelected(ofxLiteEventBoxSelected& event);
	void childBoxTriggered(ofxLiteEventBoxTriggered& event);
	
	void childGroupSingleSelected(ofxLiteEventGroupSingleSelected& event);
	void childGroupSorted(ofxLiteEventGroupSorted& event);
	
	struct{
		ofEvent<ofxLiteEventGroupSingleSelected> groupSingleSelected;
		ofEvent<ofxLiteEventGroupSorted> groupSorted;
	} groupEvents;
};

#endif
