#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{
    ofVideoGrabber webcam_feed;
    ofxCvColorImage color_frame;
    ofxCvGrayscaleImage grayscale_frame;
    ofxCvGrayscaleImage background_grayscale;
    ofxCvGrayscaleImage abs_difference;
    ofxCvContourFinder contour_finder;
    bool hasBackground = false;
    int threshold_val = 50;
    
    ofxBox2d box2d;
    vector<ofxBox2dCircle *> objects;
    ofxBox2dEdge *contour2d;
    
    string commands = "R: relearn background\n[: decrease threshold\n] increase threshold\n"
    "Click to generate a circle (inside color video only)\n-: clear all circles";
    
	public:
        /*
         * Setup method that initializes necessary images and variables.
         */
		void setup();
    
        /*
         * Update method called before every draw.
         * Checks Box2D objects and world along with the availability of a new camera frame.
         * Iterates Box2D objects and updates velocities and positions if necessary,
         * performs updates on images along with absolute differencing and blob detection.
         */
		void update();
    
        /*
         * Draws the current state of Box2D objects along with the webcam videos.
         * Displays color video along with the absolutely differenced video for the interface.
         */
		void draw();
    
        /*
         * Method that handles actions based on user key presses.
         * R: Relearn background
         * [: Decrease threshold value
         * ]: Increase threshold value
         * -: Delete all created Box2D objects
         * @param key int representing character of key pressed
         */
        void keyPressed(int key);
    
        /*
         * Method that handles mouse clicks from the user.
         * Generates Box2D circle at the location of mouse, provided the mouse is in the correct regions.
         * @param x horizontal coordinate of mouse when clicked
         * @param y vertical coordinate of mouse when clicked
         * @param button int representing mouse button clicked
         */
        void mousePressed(int x, int y, int button);

        /*
         * Method that outlines tracked contours.
         * @param polyline_contour polyline that traces contour to convert to box2dedge.
         */
        void generateEdgeContour(ofPolyline polyline_contour);
    
        /*
         * Method that clears all Box2D objects.
         */
        void clearObjects();
};
