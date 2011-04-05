#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(120);
	
	// testing getFileName before loading or setFileName
	cout << "bikers:" << bikers.getFileName() << ":" << endl;
	
	/** IMAGE LOADING TEST **/
	
	// testing setFileName and loadImage with empty parameter
	bikers.setFileName("images/bikers.jpg");
	bikers.loadImage();
	
	// testing normal image loading
	gears.loadImage("images/gears.gif");
	tdf.loadImage("images/tdf_1972_poster.jpg");
	tdfSmall.loadImage("images/tdf_1972_poster.jpg");
	tdfSmall.resize(tdf.width / 4, tdf.height / 4);
	tdfSmall.setImageType(OF_IMAGE_GRAYSCALE);
	transparency.loadImage("images/transparency.png");
	transparencyMirror.clone(transparency);
	transparencyMirror.mirror(false, true);
	bikeIcon.loadImage("images/bike_icon.png");
	bikeIcon.setImageType(OF_IMAGE_GRAYSCALE);
	
	// testing getFileName() after loading
	cout << "loaded:" << bikers.getFileName() << endl;
	cout << "loaded:" << gears.getFileName() << endl;
	cout << "loaded:" << tdf.getFileName() << endl;
	cout << "loaded:" << tdfSmall.getFileName() << endl;
	cout << "loaded:" << transparency.getFileName() << endl;
	cout << "loaded:" << bikeIcon.getFileName() << endl;
	cout << endl;
	
	/** IMAGE SAVING TEST **/
	
	// testing normal saveImage
	stringstream nameBuilder;
	char timeDateStr[255];
	sprintf(timeDateStr, "%d-%.2d-%.2d_%.2d%.2d%.2d", ofGetYear(), ofGetMonth(), ofGetDay(), ofGetHours(), ofGetMinutes(), ofGetSeconds());
	
	nameBuilder << "images/" << timeDateStr << "_bikers.jpg";
	bikers.saveImage(nameBuilder.str());
	nameBuilder.str("");
	
	// testing setFileName and saveImage with empty parameter
	nameBuilder << "images/" << timeDateStr << "_gears.png";
	gears.setFileName(nameBuilder.str());
	gears.saveImage();
	nameBuilder.str("");
	
	// testing getFileName() after saving
	cout << "saved:" << bikers.getFileName() << endl;
	cout << "saved:" << gears.getFileName() << endl;
}


//--------------------------------------------------------------
void testApp::update(){
	ofBackground(255,255,255);	
}


//--------------------------------------------------------------
void testApp::draw(){	
	ofSetColor(0xFFFFFF);

	bikers.draw(0,0);
	gears.draw(600,0);
	tdf.draw(600,300);
	
	ofSetColor(0xDD3333);
	tdfSmall.draw(200,300);
	
	ofSetColor(0xFFFFFF);
	ofEnableAlphaBlending();
	transparency.draw(sin(ofGetElapsedTimeMillis()/1000.0f) * 100 + 500,20);
	ofSetColor(255, 255, 255, 192);
	transparencyMirror.draw(sin(ofGetElapsedTimeMillis()/1000.0f) * 100 + 500,20+transparency.getHeight());
	ofDisableAlphaBlending();
	
	
	ofSetColor(0x000000);
	
	// getting the pixels out of an image, 
	// and then use the values to draw circles
	unsigned char * pixels = bikeIcon.getPixels();
	int w = bikeIcon.width;
	int h = bikeIcon.height;
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			int value = pixels[j * w + i];
			float pct = 1 - (value / 255.0f);
			ofCircle(i*10,500 + j*10,1 + 5*pct);		
		}
	}
	
	ofSetColor(0xFFFFFF);
	bikeIcon.draw(300,500, 20,20);
	
	// rotate a copy of the icon by 5 pixels every 10 frames
	if(ofGetFrameNum() % 10 == 0){
		bikeIconRot.setFromPixels(pixels, bikeIcon.getWidth(), bikeIcon.getHeight(), bikeIcon.type);
		bikeIconRot.rotate(5.0 * ofGetFrameNum()/10);
	}
	bikeIconRot.draw(360-bikeIconRot.getWidth()/2,510-bikeIconRot.getHeight()/2); // drawing it with center at 360,510
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
