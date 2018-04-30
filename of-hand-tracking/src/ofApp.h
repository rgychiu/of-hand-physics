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
    vector<ofPolyline *> blob_edges;
    bool hasBackground = false;
    int threshold_val = 50;
    
    ofxBox2d box2d; // box2d world object
    vector<ofxBox2dCircle *> objects; // generated circle objects
    vector<ofxBox2dEdge *> contour2d; // edges generated from contour
    
	public:
		void setup();
		void update();
		void draw();

        void generateNewContour(vector<ofxCvBlob> blobs); // Generate polylines from tracked blobs
        void generateNew2dContour(vector<ofPolyline *> polyline_contour); // Generated ofxbox2d edges from polylines
        void clearObjects(); // Clear all circle objects
    
		void keyPressed(int key);
        void mousePressed(int x, int y, int button);
};
