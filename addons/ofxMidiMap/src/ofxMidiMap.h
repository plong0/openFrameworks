/*
 *  ofxMidiMap.h
 *
 *  Created by Pat Long (plong0) on 11-03-30.
 *  Copyright 2011 Spiral Sense. All rights reserved.
 *
 */
#ifndef _OFX_MIDI_MAP_H
#define _OFX_MIDI_MAP_H

#include "ofxLiteGUI.h"
#include "ofxMidi.h"

class ofxMidiMapValueUpdateEvent : public ofEventArgs{
public:
	string name;
	int* value;
};

class ofxMidiMap{
protected:
	// Generic display of a name, value pair
	class LinkedBox : public ofxLiteBox{
		public:
			LinkedBox(string name="Value", string value="");
			
			void drawLinkBox(bool fill, float x=-1.0, float y=-1.0);
			virtual void drawContent(float x=-1.0, float y=-1.0);
	};
		
	class MidiControlDisplay; // forward declaration
	
	// Displays a controlled value
	class ControlledValueDisplay : public LinkedBox{
		protected:
			ofxMidiMap* midiMap; // needed for triggering events
			int* controlValue;
			
			MidiControlDisplay* midiValue;
			
		public:
			ControlledValueDisplay(string name, int* controlValue);
			
			virtual void drawContent(float x=-1.0, float y=-1.0);
		
			MidiControlDisplay* getMidiValue();
		
			void setMidiMap(ofxMidiMap* midiMap);
			void setMidiValue(MidiControlDisplay* midiValue);
			virtual void setSelected(bool selected);
			virtual void setValue(string value);
	};
	
	// Displays a Midi Controller value
	class MidiControlDisplay : public LinkedBox{
		protected:
			map<string,ControlledValueDisplay*> controlledValues;
		
		public:
			MidiControlDisplay(string name, string value="");
			
			virtual void drawContent(float x=-1.0, float y=-1.0);
			
			bool addControlledValue(ControlledValueDisplay* controlValue);
			bool removeControlledValue(ControlledValueDisplay* controlValue);
			
			virtual void setSelected(bool selected);
			virtual void setValue(string value);
	};
	
	ofxMidiIn* midiIn;
	
	ofxLiteGroup controlledValues;
	ofxLiteGroup midiValues;
	
	ControlledValueDisplay* selectedControlled;
	MidiControlDisplay* selectedMidiControl;
	
public:
	ofxMidiMap();
	~ofxMidiMap();
	
	virtual void draw();
	virtual void update();
	
	void midiMessage(ofxMidiEventArgs &args);
	
	void addControllableValue(string name, int* value);
	void setMidiIn(ofxMidiIn* midiIn);
	
	bool mouseMoved(int x, int y);
	bool mouseDragged(int x, int y, int button);
	bool mousePressed(int x, int y, int button);
	bool mouseReleased(int x, int y, int button);
	
	ofEvent<ofxMidiMapValueUpdateEvent> valueUpdate;
};

#endif
