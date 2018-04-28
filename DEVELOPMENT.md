# Understanding Image Processing and Computer Vision with OpenFrameworks

**_The first step involved understanding the concepts behind tracking and computer vision in OF_**.
## Images
* Comprised of one-dimensional array of integers representing color
* In OpenCv, images can come from a variety of locations (disk, webcam (ofVideoGrabber), pixels of frame from screen, etc.)
* In designing the application with OF, the update() function can become inefficient (reading from disk repeatedly, updating without new frame, etc.)

## Image Varieties
* 1 Channel (Grayscale) images - only one byte to represent each pixel, ranges from 0 (black) to 255 (white)
* 3 Channel (RGB) images - each pixel involves 3 bytes of data, 1 byte for each color (RGB)
* 4 Channel (RGBA) images - each pixel involves 4 bytes of data, also encodes alpha value (opacity)
* etc.

## Computer Vision and Image Processing
* Working with 1 channel images improves speed of processing, but also eliminates unnecessary intermediate steps.
* OpenCv takes colorimetric coefficients to convert to grayscale images - based on assumption that humans see certain colors more than others and allow for more accurate conversions
* Grayscale images can be compared (absolute differencing) to see the greatest difference between two images, and this idea leads to the ability to track and other ideas in computer vision.
* Comparison between background grayscale image and background + object image = determine foreground vs. background
* Greater difference between pixels = foreground, outline of foreground allows blobs to be formed and traced = tracking

# Development and Bugs
**_The next step involved developing the program_**
## Design Choices
* Very little interaction between the user and the program in terms of key presses = no need for functions other than setup, update, and draw
* Background needs to be captured with the first available frame - allows for little disruption of background and automatic thresholding and differencing with the introduction of live foreground objects (using a purely black or white background would produce flaws with different backgrounds and inaccurate results for different pixels)
* Sizes of images and video in the application are being allocated purely for 'testing' purposes. 'Proper' test cases can't be written because the image manipulation utilizes functions of the library, which can be trusted

## Progress
* Utilizes live webcam images for tracking
* Able to learn background and difference images
* Tracking accomplished through contour finding
* Box2D library implemented

## Issues
* One problem that was encountered was using an ofVideoGrabber and attempting to convert to a grayscale webcam feed, which led to incompatible types - solved by using an image that was updated with every frame from the feed; grayscale image had an overloaded operator that led to automatic conversion
* Absolute differencing produced image with background still present along with foreground in some cases
* Global threshold inefficient for changing background or changing lighting as well as contour finding, allowing users to adjust threshold values or implementing automatic threshold selection can solve issue
* Having program capture first available frame for background and have it be unchangeable decreases usability - users have to restart the program for a new background. Including options to relearn the background with a keypress or interface solves this problem

# Sources
* http://openframeworks.cc/ofBook/chapters/image_processing_computer_vision.html
* http://openframeworks.cc/documentation/ofxOpenCv/ofxCvColorImage/
* http://openframeworks.cc/documentation/video/ofVideoGrabber/
* http://openframeworks.cc/documentation/ofxOpenCv/ofxCvGrayscaleImage/
* http://openframeworks.cc/documentation/ofxOpenCv/ofxCvContourFinder/
