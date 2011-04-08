/*
 *  ofxLiteEvent.cpp
 *  emptyExample
 *
 *  Created by Eli Smiles on 11-04-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "ofxLiteEvent.h"

ofxLiteEvent::ofxLiteEvent(ofxLiteBox* lite){
	this->lite = lite;
	this->liteName = "";
	if(lite != NULL) this->liteName = lite->getName();
}
