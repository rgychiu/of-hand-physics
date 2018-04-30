#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{
    ofVideoGrabber webcam_feed; // use laptop webcam to get feed of pixels and updates
    ofxCvColorImage color_frame; // webcam frame with 3 channel colors
    ofxCvGrayscaleImage grayscale_frame; // webcam frame with colors removed - convert to monochrome frame
    ofxCvGrayscaleImage background_grayscale; // grayscaled background image used for differencing
    ofxCvGrayscaleImage abs_difference; // grayscaled absolute difference image used for tracking
    ofxCvContourFinder contour_finder; // find, store, and trace tracked blobs/hand on the screen
    vector<ofPolyline *> blob_edges; // edges/outline of blobs found from contourfinder
    bool hasBackground = false;
    int threshold_val = 50;
    
    ofxBox2d box2d;
    vector<shared_ptr<ofxBox2dCircle>> objects;
    vector<ofxBox2dEdge *> contour2d;
    
	public:
		void setup();
		void update();
		void draw();

        // Generate contour from blobs found using polyline - allows for box2d interaction
        void generateContour(vector<ofxCvBlob> blobs);
        void generate2dContour(vector<ofPolyline *> polyline_contour);
    
		void keyPressed(int key);
        void mousePressed(int x, int y, int button);
};
