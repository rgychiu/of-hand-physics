#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{
    ofVideoGrabber webcam_feed; // Use laptop webcam to get feed of pixels and updates
    ofxCvColorImage color_frame; // webcam frame with 3 channel colors
    ofxCvGrayscaleImage grayscale_frame; // webcam frame with colors removed - convert to monochrome frame
    ofxCvGrayscaleImage background_grayscale; // grayscaled background image used for differencing
    ofxCvGrayscaleImage abs_difference; // grayscaled absolute difference image used for tracking
    ofxCvContourFinder contour_finder; // find, store, and trace tracked blobs/hand on the screen
    bool hasBackground = false;
    int threshold_val = 50;
    
    ofxBox2d box2d;
    vector<shared_ptr<ofxBox2dCircle>> objects;
    
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
        void mousePressed(int x, int y, int button);
//        void keyReleased(int key);
//        void mouseMoved(int x, int y );
//        void mouseDragged(int x, int y, int button);
//        void mouseReleased(int x, int y, int button);
//        void mouseEntered(int x, int y);
//        void mouseExited(int x, int y);
//        void windowResized(int w, int h);
//        void dragEvent(ofDragInfo dragInfo);
//        void gotMessage(ofMessage msg);
		
};
