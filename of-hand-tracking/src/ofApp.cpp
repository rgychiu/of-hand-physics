#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    webcam_feed.initGrabber(320, 240); // Use default capture device, window size set to 320x240 px (supported size)
    
    // Set sizes of each image that correspond to video feed - prevent compression errors and other bugs
    color_frame.allocate(320, 240);
    grayscale_frame.allocate(320, 240);
    background_grayscale.allocate(320, 240);
    abs_difference.allocate(320, 240);
    
    // Initialize box2d world and objects
    box2d.init();
    box2d.setGravity(0, 10);
    box2d.createGround(0, 700, 1024, 710);
}

//--------------------------------------------------------------
void ofApp::update(){
    box2d.update();
    
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
        
        // Find blobs in image
        // Consider 1 blob since hand (and even body) needs to be one whole region
        // Finding holes false since the outline of hand is desired
        // Want the tracked blob to be significant - large enough to distinguish from background but not entire screen
        int minArea = abs_difference.getWidth() * abs_difference.getHeight() * 0.05;
        int maxArea = abs_difference.getWidth() * abs_difference.getHeight() * 0.75;
        contour_finder.findContours(abs_difference, minArea, maxArea, 1, false);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Display color frames/feeds on screen with one corner at (x, y) for 'testing'
    color_frame.draw(10, 10);
    grayscale_frame.draw(350, 10);
    background_grayscale.draw(10, 270);
    abs_difference.draw(350, 270);
    contour_finder.draw();
    
    // Check if objects available to generate and draw
    for (auto object : objects) {
        object->draw();
    }
    
    // Draw string instructions for better interface/usability
    string instructions = "R: relearn background\n[: decrease threshold\n] increase threshold\n"
                          "Click to generate a circle\n-: delete circles";
    ofDrawBitmapString(instructions, 10, 530);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // Keyboard shortcuts to help with hand tracking methods
    // Normalize key press to uppercase
    char char_pressed = toupper(key);

    // Relearn/reset background - R
    // Set threshold value - [, ]
    if (char_pressed == 'R') {
        // Get current webcam frame and make new background
        background_grayscale = color_frame;
    } else if (char_pressed == '[') {
        threshold_val--;
    } else if (char_pressed == ']') {
        threshold_val++;
    } else if (char_pressed == '-') {
        if (!objects.empty()) {
            objects.erase(objects.begin(), objects.begin() + 1);
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    auto circle = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
    circle->setPhysics(0.5, 0.5, 1);
    circle->setup(box2d.getWorld(), x, y, 20);
    objects.emplace_back(circle);
}
