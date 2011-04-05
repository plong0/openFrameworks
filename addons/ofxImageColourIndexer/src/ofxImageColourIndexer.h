/*
 *  ofxImageColourIndexer.h
 *  emptyExample
 *
 *  Created by Eli Smiles on 11-03-16.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"

struct ColourCount{
	ofColor colour;
	int count;
	
	bool operator()(ColourCount* c1, ColourCount* c2){
		return (c1->count > c2->count);
	}
};

class ofxImageColourIndexer{
protected:
	ofImage myImage;
	ofColor averageColour;
	ofColor popularColour;
	vector<ColourCount*> colours;
	
	int popularAverageCount;
	float popularAveragePercent;
	float popularThreshLow, popularThreshHigh;
	
	bool hasAverage, hasPopular;
	
public:
	ofxImageColourIndexer(ofImage image);
	~ofxImageColourIndexer();
	
	void draw(float x, float y, float w=-1.0, float h=-1.0);
	
	void calculateAverageColour();
	void calculatePopularColour();
	
	ofColor getAverageColour(bool doCalculate=true);
	ofColor getPopularColour(bool doCalculate=true);
	ofColor getPopularAverageColour(int useColours, bool doCalculate=true);
	ofColor getPopularAverageColour(float usePercent, bool doCalculate=true);
	
	bool isAverageCalculated();
	bool isPopularCalculated();
	
	void setPopularAverageCount(int popularAverageCount=1);
	void setPopularAveragePercent(float popularAveragePercent=0.0);
	void setPopularThreshold(float popularThreshLow=0.0, float popularThreshHigh=255.0);
	
};
