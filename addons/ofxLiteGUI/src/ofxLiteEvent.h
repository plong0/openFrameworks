/*
 *  ofxLiteEvent.h
 *  emptyExample
 *
 *  Created by Eli Smiles on 11-04-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _OFX_LITE_GUI_EVENT
#define _OFX_LITE_GUI_EVENT

#include "ofMain.h"

class ofxLiteBox;

class ofxLiteEvent : public ofEventArgs{
public:
	ofxLiteBox* lite;
	string liteName;
	
	ofxLiteEvent(ofxLiteBox* lite=NULL);
};

#include "ofxLiteBox.h"

#endif
