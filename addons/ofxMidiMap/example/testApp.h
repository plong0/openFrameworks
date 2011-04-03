#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxMidiMap.h"

class testApp : public ofBaseApp{
	protected:
		ofxMidiIn midiIn;
		ofxMidiMap midiMap;
	
		int val1;
		int val2;
		int val3;
		int val4;

	public:
		void setup();
		void update();
		void draw();
	
		void midiMapValueChanged(ofxMidiMapValueUpdateEvent &event);

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

};

#endif
