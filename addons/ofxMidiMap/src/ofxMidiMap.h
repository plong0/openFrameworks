/*
 *  ofxMidiMap.h
 *
 *  Created by Pat Long (plong0) on 11-03-30.
 *  Copyright 2011 Spiral Sense. All rights reserved.
 *
 */
#ifndef _OFX_MIDI_MAP_H
#define _OFX_MIDI_MAP_H

#include "ofxMidi.h"

class ofxMidiMapValueUpdateEvent : public ofEventArgs{
public:
	string name;
	int* value;
};

class ofxMidiMap{
protected:
	// Generic display of a name, value pair
	struct ValueDisplay{
		string name;
		string value;
		ofRectangle bounds;
		
		bool hover;
		bool selected;
		
		ValueDisplay(string name, string value="");
		
		void drawHoverBox();
		void drawLinkBox(bool fill);
		void drawSelectedBox();
		virtual void draw(float x, float y);
		virtual void update();
		
		virtual void setSelected(bool selected);
		virtual void setValue(string value);
		
		bool inBounds(int x, int y);
		bool isHovered();
		bool isSelected();
		
		virtual bool mouseMoved(int x, int y);
		virtual bool mouseDragged(int x, int y, int button);
		virtual bool mousePressed(int x, int y, int button);
		virtual bool mouseReleased(int x, int y, int button);
	};
	
	// Generic display of a group of values
	struct ValueDisplayGroup{
		string name;
		map<string,ValueDisplay*> values;
		
		ValueDisplayGroup(string name="");
		~ValueDisplayGroup();
		
		void draw(float x, float y);
		void update();
		
		ValueDisplay* addValue(string name, ValueDisplay* value);
		void setName(string name);
		void setValue(string name, string value);
		
		bool mouseMoved(int x, int y);
		bool mouseDragged(int x, int y, int button);
		bool mousePressed(int x, int y, int button);
		bool mouseReleased(int x, int y, int button);
	};
	
	struct MidiControlDisplay; // forward declaration
	
	// Displays a controlled value
	struct ControlledValueDisplay : ValueDisplay{		
		ofxMidiMap* midiMap; // needed for triggering events
		int* controlValue;
		
		MidiControlDisplay* midiValue;
		
		ControlledValueDisplay(string name, int* controlValue);
		
		virtual void draw(float x, float y);
		
		virtual void setSelected(bool selected);
		virtual void setValue(string value);
	};
	
	// Displays a Midi Controller value
	struct MidiControlDisplay : ValueDisplay{
		map<string,ControlledValueDisplay*> controlledValues;
		
		MidiControlDisplay(string name, string value="");
		
		virtual void draw(float x, float y);
		
		bool addControlledValue(ControlledValueDisplay* controlValue);
		bool removeControlledValue(ControlledValueDisplay* controlValue);
		
		virtual void setSelected(bool selected);
		virtual void setValue(string value);
	};
	
	ofxMidiIn* midiIn;
	
	ValueDisplayGroup controlledValues;
	ValueDisplayGroup midiValues;
	
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
