#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Use default capture device, set to 640x480 px (supported size)
    webcam_feed.initGrabber(640, 480);
    
    // Set dynamic sizes that correspond to video feed - prevent region bugs
    color_frame.allocate(webcam_feed.getWidth(), webcam_feed.getHeight());
    grayscale_frame.allocate(webcam_feed.getWidth(), webcam_feed.getHeight());
    background_grayscale.allocate(webcam_feed.getWidth(), webcam_feed.getHeight());
    abs_difference.allocate(webcam_feed.getWidth(), webcam_feed.getHeight());
    
    // Initialize box2d world and objects, set gravity in world
    box2d.init();
    box2d.setGravity(0, 10);
    
    // Create grounds and walls of world to contain only color feed region
    box2d.createGround();
    box2d.createBounds(0, 0, webcam_feed.getWidth(), webcam_feed.getHeight());
}

//--------------------------------------------------------------
void ofApp::update(){
    box2d.update();
    webcam_feed.update();
    // Check that there is a new frame to load - reduces amount of work by constant updates
    // If there is a new frame, update color and grayscaled images to continue producing a video
    if (webcam_feed.isFrameNew()){
        // Default set background image for differencing as first available frame
        if (!hasBackground) {
            color_frame.setFromPixels(webcam_feed.getPixels());
            background_grayscale = color_frame;
            hasBackground = !hasBackground;
        }
        
        color_frame.setFromPixels(webcam_feed.getPixels());
        grayscale_frame = color_frame;
        
        // Get absolute difference between two frames to obtain foreground and background
        abs_difference.absDiff(background_grayscale, grayscale_frame);
        abs_difference.threshold(threshold_val);
        abs_difference.dilate();
        abs_difference.dilate();
        
        // Find 1 blob since hand (and even body) should be one whole region
        int minArea = abs_difference.getWidth() * abs_difference.getHeight() * 0.05;
        int maxArea = abs_difference.getWidth() * abs_difference.getHeight() * 0.75;
        contour_finder.findContours(abs_difference, minArea, maxArea, 1, false);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Display color frames/feeds on screen with one corner at (x, y) for 'testing'
    color_frame.draw(0, 0);
    abs_difference.draw(webcam_feed.getWidth() + 1, 0);
    
    // Check that blob exists in frame and draw outline
    if (!contour_finder.blobs.empty()) {
        generateEdgeContour(ofPolyline(contour_finder.blobs.at(0).pts));
        contour2d->draw();
    }
    
    // Check if box2d objects available to draw
    for (auto object : objects) {
        object->draw();
    }
    
    // Draw string instructions for better interface/usability
    ofDrawBitmapString(commands, 10, webcam_feed.getHeight() + 50);
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
    // Prevent generation outside of box2d world
    if (x < webcam_feed.getWidth() && y < webcam_feed.getHeight()) {
        auto circle = new ofxBox2dCircle();
        circle->setPhysics(0.5, 0.5, 1);
        circle->setup(box2d.getWorld(), x, y, 20);
        objects.emplace_back(circle);
    }
}

//--------------------------------------------------------------
void ofApp::generateEdgeContour(ofPolyline polyline_contour) {
    // Take polylines that outline blobs and convert to box2d shapes that can interact
    delete contour2d;
    contour2d = new ofxBox2dEdge();
    contour2d->addVertexes(polyline_contour);
    contour2d->create(box2d.getWorld());
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
