#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Use default capture device, window size set to 320x240 px (supported size)
    webcam_feed.initGrabber(640, 480);
    
    // Set sizes of each image that correspond to video feed - prevent compression errors and other bugs
    color_frame.allocate(640, 480);
    grayscale_frame.allocate(640, 480);
    background_grayscale.allocate(640, 480);
    abs_difference.allocate(640, 480);
    
    // Initialize box2d world and objects, set gravity in world
    box2d.init();
    box2d.setGravity(0, 10);
    
    // Create grounds and walls of world (same as height and width of window currently)
    box2d.createGround();
    box2d.createBounds(0,0,640,480);
}

//--------------------------------------------------------------
void ofApp::update(){
    // Update image with next frame/image (how videos are produced)
    webcam_feed.update();
    // Check that there is a new frame to load - reduces amount of work by constant updates
    // If there is a new frame, update color and grayscaled images to continue producing a video
    if (webcam_feed.isFrameNew()){
        // Set background image for differencing as first available frame - seemingly automatic threshholding and tracking
        if (!hasBackground) {
            color_frame.setFromPixels(webcam_feed.getPixels());
            background_grayscale = color_frame;
            hasBackground = !hasBackground;
        }
        
        // Otherwise continue to get color and grayscale live feed
        color_frame.setFromPixels(webcam_feed.getPixels());
        grayscale_frame = color_frame;
        
        // Get absolute difference between two frames to obtain foreground and background
        abs_difference.absDiff(background_grayscale, grayscale_frame);
        abs_difference.threshold(threshold_val);
        abs_difference.dilate();
        abs_difference.dilate();
        
        // Find 1 blob since hand (and even body) needs to be one whole region
        // Finding holes false since the outline of hand is desired
        int minArea = abs_difference.getWidth() * abs_difference.getHeight() * 0.05;
        int maxArea = abs_difference.getWidth() * abs_difference.getHeight() * 0.75;
        contour_finder.findContours(abs_difference, minArea, maxArea, 1, false);
    }
    
    // Update box2d objects (position and velocities)
    box2d.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Display color frames/feeds on screen with one corner at (x, y) for 'testing'
    color_frame.draw(0, 0);
    abs_difference.draw(641, 0);
    
    // Get all points in the contour and connect together using polylines
    generateNewContour(contour_finder.blobs);
    generateNew2dContour(blob_edges);
    for (auto box_outline : contour2d) {
        box_outline->draw();
    }
    
    // Check if objects available to generate and draw
    for (auto object : objects) {
        object->draw();
    }
    
    // Draw string instructions for better interface/usability
    string instructions = "R: relearn background\n[: decrease threshold\n] increase threshold\n"
                          "Click to generate a circle\n-: clear all circles";
    ofDrawBitmapString(instructions, 10, 580);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // Normalize key press to uppercase
    char char_pressed = toupper(key);

    if (char_pressed == 'R') {
        // Get current webcam frame and make new background
        background_grayscale = color_frame;
    } else if (char_pressed == '[') {
        threshold_val--;
    } else if (char_pressed == ']') {
        threshold_val++;
    } else if (char_pressed == '-' && !objects.empty()) {
        // Delete ALL circle objects if there are circles on screen
        clearObjects();
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    // Generate box2dCircle object, set physics and finally place in world to start moving
    auto circle = new ofxBox2dCircle();
    circle->setPhysics(0.5, 0.5, 1);
    circle->setup(box2d.getWorld(), x, y, 20);
    objects.emplace_back(circle);
}

//--------------------------------------------------------------
void ofApp::generateNewContour(vector<ofxCvBlob> blobs) {
    // Clear previous blobs (allows for constant updates and blob redrawing)
    vector<ofPolyline *> empty;
    for (auto prev_tracked : blob_edges) {
        delete prev_tracked;
    }
    blob_edges.clear();
    blob_edges.swap(empty);
    
    // Each contour has a vector of blobs that it found
    // Each blob has a vector of ofPoints that can be used for polylines
    for (auto obj : blobs) {
        blob_edges.emplace_back(new ofPolyline(obj.pts));
    }
}

//--------------------------------------------------------------
void ofApp::generateNew2dContour(vector<ofPolyline *> polyline_contour) {
    // Clear previous edges (allow for constant updates with contour)
    vector<ofxBox2dEdge *> empty;
    for (auto prev_edges : contour2d) {
        delete prev_edges;
    }
    contour2d.clear();
    contour2d.swap(empty);
    
    // Take polylines that outline blobs and convert to box2d shapes that can interact
    for (auto contour_line : polyline_contour) {
        auto box_contour = new ofxBox2dEdge();
        box_contour->addVertexes(*contour_line);
        box_contour->create(box2d.getWorld());
        contour2d.emplace_back(box_contour);
    }
}

//--------------------------------------------------------------
void ofApp::clearObjects() {
    // Iterate and delete all created objects
    vector<ofxBox2dCircle *> empty;
    for (auto circle : objects) {
        delete circle;
    }
    objects.clear();
    objects.swap(empty);
}
