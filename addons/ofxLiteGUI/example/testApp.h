#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxLiteGUI.h"

class testApp : public ofBaseApp{
	protected:
		ofxLiteGroup liteGroup;

	public:
		void setup();
		void update();
		void draw();
	
		void liteBoxDragged(ofxLiteEventBoxDragged& event);
		void liteBoxSelected(ofxLiteEventBoxSelected& event);
		void liteGroupSorted(ofxLiteEventGroupSorted& event);

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

};

#endif
