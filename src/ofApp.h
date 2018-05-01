#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{
    ofVideoGrabber webcam_feed;
    ofxCvColorImage color_frame; // webcam frame with 3 channel colors
    ofxCvGrayscaleImage grayscale_frame; // webcam frame with colors removed - convert to monochrome frame
    ofxCvGrayscaleImage background_grayscale; // grayscaled background image used for differencing
    ofxCvGrayscaleImage abs_difference; // grayscaled absolute difference image used for tracking
    ofxCvContourFinder contour_finder; // find, store, and trace tracked blobs/hand on the screen
    bool hasBackground = false;
    int threshold_val = 50;
    string commands = "R: relearn background\n[: decrease threshold\n] increase threshold\n"
                      "Click to generate a circle (inside color video only)\n-: clear all circles";
    
    ofxBox2d box2d;
    vector<ofxBox2dCircle *> objects;
    ofxBox2dEdge *contour2d;
    
	public:
        // OpenFrameworks functions
		void setup();
		void update();
		void draw();
        void keyPressed(int key);
        void mousePressed(int x, int y, int button);

        // Helper functions
        void generateEdgeContour(ofPolyline polyline_contour); // Generated ofxbox2d edges from polylines
        void clearObjects(); // Clear all circle objects
};
