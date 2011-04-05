/*
 *  ofxImageColourIndexer.cpp
 *  emptyExample
 *
 *  Created by Eli Smiles on 11-03-16.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxImageColourIndexer.h"

ofxImageColourIndexer::ofxImageColourIndexer(ofImage image){
	myImage = image;
	hasAverage = false;
	hasPopular = false;
	setPopularAverageCount();
	setPopularAveragePercent();
	setPopularThreshold();
}

ofxImageColourIndexer::~ofxImageColourIndexer(){
	for(int i=0; i < colours.size(); i++){
		delete colours[i];
		colours[i] = NULL;
	}
	colours.clear();
}

void ofxImageColourIndexer::draw(float x, float y, float w, float h){
	if(w < 0.0) w = myImage.getWidth();
	if(h < 0.0) h = myImage.getHeight();
	myImage.draw(x, y, w, h);
}

void ofxImageColourIndexer::calculateAverageColour(){
	unsigned char* pixels = myImage.getPixels();
	int width = myImage.getWidth();
	int height = myImage.getHeight();
	int bpp = myImage.bpp / 8;
	averageColour.r = averageColour.g = averageColour.b = 0.0;
	for(int i=0; i<width; i++){
		for(int j=0; j<height; j++){
			averageColour.r += pixels[ (j*width+i)*bpp + 0];
			averageColour.g += pixels[ (j*width+i)*bpp + 1];
			averageColour.b += pixels[ (j*width+i)*bpp + 2];
		}
	}
	int pixCount = width*height;
	averageColour.r /= (float)pixCount;
	averageColour.g /= (float)pixCount;
	averageColour.b /= (float)pixCount;
	hasAverage = true;
}

void ofxImageColourIndexer::calculatePopularColour(){
	unsigned char* pixels = myImage.getPixels();
	int width = myImage.getWidth();
	int height = myImage.getHeight();
	int bpp = myImage.bpp / 8;
	
	map<string,ColourCount*> colourCounts;
	for(int i=0; i<width; i++){
		for(int j=0; j<height; j++){
			ofColor cColor;
			cColor.r = pixels[ (j*width+i)*bpp + 0];
			cColor.g = pixels[ (j*width+i)*bpp + 1];
			cColor.b = pixels[ (j*width+i)*bpp + 2];
			
			float cAvg = (float)(cColor.r + cColor.g + cColor.b) / 3.0;
			bool rThresh = (cColor.r >= popularThreshLow && cColor.r <= popularThreshHigh);
			bool gThresh = (cColor.g >= popularThreshLow && cColor.g <= popularThreshHigh);
			bool bThresh = (cColor.b >= popularThreshLow && cColor.b <= popularThreshHigh);
			bool avgThresh = (cAvg >= popularThreshLow && cAvg <= popularThreshHigh);
			
			if(rThresh || gThresh || bThresh || avgThresh){
				stringstream nameBuild;
				nameBuild << cColor.r << "," << cColor.g << "," << cColor.b;
				if(colourCounts.find(nameBuild.str()) == colourCounts.end()){
					ColourCount* newOne = new ColourCount;
					newOne->colour = cColor;
					newOne->count = 1;
					colourCounts[nameBuild.str()] = newOne;
					colours.push_back(newOne);
				}
				else{
					colourCounts[nameBuild.str()]->count++;
				}
			}
		}
	}
	
	if(colours.size() > 0){
		ColourCount sorter;
		sort(colours.begin(), colours.end(), sorter);
		popularColour = colours[0]->colour;
		
		if(popularAveragePercent > 0.0)
			popularColour = getPopularAverageColour(popularAveragePercent);
		else if(popularAverageCount > 1)
			popularColour = getPopularAverageColour(popularAverageCount);
		
	}
	hasPopular = true;
}

ofColor ofxImageColourIndexer::getAverageColour(bool doCalculate){
	if(!hasAverage && doCalculate)
		calculateAverageColour();
	return averageColour;
}

ofColor ofxImageColourIndexer::getPopularColour(bool doCalculate){
	if(!hasPopular && doCalculate)
		calculatePopularColour();
	return popularColour;
}

ofColor ofxImageColourIndexer::getPopularAverageColour(int useColours, bool doCalculate){
	if(!hasPopular && doCalculate)
		calculatePopularColour();
	ofColor popularAverage = popularColour;
	if(useColours > 0){
		popularAverage.r = popularAverage.g = popularAverage.b = 0.0;
		int usedColours = 0;
		for(int i=0; i < useColours && i < colours.size(); i++){
			popularAverage.r += colours[i]->colour.r;
			popularAverage.g += colours[i]->colour.g;
			popularAverage.b += colours[i]->colour.b;
			usedColours++;
		}
		
		if(usedColours > 0){
			popularAverage.r /= (float)usedColours;
			popularAverage.g /= (float)usedColours;
			popularAverage.b /= (float)usedColours;
		}
	}
	
	return popularAverage;
}

ofColor ofxImageColourIndexer::getPopularAverageColour(float usePercent, bool doCalculate){
	if(!hasPopular && doCalculate)
		calculatePopularColour();
	if(usePercent > 1.0) usePercent / 100.0;
	int colourCount = (int)((float)colours.size() * usePercent);
	return getPopularAverageColour(colourCount, doCalculate);
}

bool ofxImageColourIndexer::isAverageCalculated(){
	return hasAverage;
}

bool ofxImageColourIndexer::isPopularCalculated(){
	return hasPopular;
}

void ofxImageColourIndexer::setPopularAverageCount(int popularAverageCount){
	this->popularAverageCount = popularAverageCount;
}

void ofxImageColourIndexer::setPopularAveragePercent(float popularAveragePercent){
	if(popularAveragePercent > 1.0) popularAveragePercent /= 100.0;
	this->popularAveragePercent = popularAveragePercent;
}

void ofxImageColourIndexer::setPopularThreshold(float popularThreshLow, float popularThreshHigh){
	this->popularThreshLow = popularThreshLow;
	this->popularThreshHigh = popularThreshHigh;
}
