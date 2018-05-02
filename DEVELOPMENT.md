# Development
## Understanding Image Processing and Computer Vision with OpenFrameworks
**_The first step involved understanding the concepts behind tracking and computer vision in OF_**.
### Images
* Comprised of one-dimensional array of integers representing color
* In OpenCv, images can come from a variety of locations (disk, webcam (ofVideoGrabber), pixels of frame from screen, etc.)
* In designing the application with OF, the update() function can become inefficient (reading from disk repeatedly, updating without new frame, etc.)

### Image Varieties
* 1 Channel (Grayscale) images - only one byte to represent each pixel, ranges from 0 (black) to 255 (white)
* 3 Channel (RGB) images - each pixel involves 3 bytes of data, 1 byte for each color (RGB)
* 4 Channel (RGBA) images - each pixel involves 4 bytes of data, also encodes alpha value (opacity)
* etc.

### Computer Vision and Image Processing
* Working with 1 channel images improves speed of processing, but also eliminates unnecessary intermediate steps.
* OpenCv takes colorimetric coefficients to convert to grayscale images - based on assumption that humans see certain colors more than others and allow for more accurate conversions
* Grayscale images can be compared (absolute differencing) to see the greatest difference between two images, and this idea leads to the ability to track and other ideas in computer vision.
* Comparison between background grayscale image and background + object image = determine foreground vs. background
* Greater difference between pixels = foreground, outline of foreground allows blobs to be formed and traced = tracking

## Development and Bugs
**_The next step involved developing the program_**
### Design Choices
* Very little interaction between the user and the program in terms of key presses
   * No need for functions other than setup, update, and draw
* Background needs to be captured with the first available frame
   * Allows for little disruption of background and automatic thresholding and differencing with the introduction of live foreground objects (using a purely black or white background would produce flaws with different backgrounds and inaccurate results for different pixels)
   * Still allow user to reset background in the case that the background changes
* 'Proper' test cases can't be written because the image manipulation utilizes functions of the library, which can be trusted
* All four frames need to be shown because of the interface of choosing to relearn the background and changing the threshold - needed for better usability.
  * Given the ability to adjust threshold and reset backgruond, only the color and absolutely differenced frames can be shown
     * Allows for larger Box2d world
* The size for each window (640x480) needed to be used because there are a limited amount of sizes supported by OF.
* Interface and resetting the background allows for just color and absolute differencing videos to only be shown
* Allow object clearing such that users can continue to add and remove objects
* Sizes of each frame and other images set to match webcam feed dimensions and responsive layout
   * Prevents errors if video size isn't supported, default set to 640x480 (common supported size, max supported size that can allow two images to fit on one screen)
* Threshold has default value because of absolute differencing, can be changed after program runs

### Progress/Features
- [x] Utilize laptop webcam for tracking
- [x] Convert laptop feed to 1 channel feed
- [x] Capture single frame from 1 channel feed to use for background
- [x] Absolute difference 1 channel feed with background frame
- [x] Draw contours surrounding body to be tracked
- [x] Create basic interface to change different parameters without restarting program
- [x] Include Box2d addon
- [x] Create Box2d world
- [x] Include and allow creation of basic shape in window
- [x] Allow interaction between shapes and tracked contours
- [x] Confine Box2d world to webcam feed
- [x] Resizing support/Handle unsupported video size
- [x] Clean up code
- [x] Commenting, style fixes

### Issues
* One problem that was encountered was using an ofVideoGrabber and attempting to convert to a grayscale webcam feed, which led to incompatible types
   * Solved by using an image that was updated with every frame from the feed; grayscale image had an overloaded operator that led to automatic conversion
* Absolute differencing produced image with background still present along with foreground in some cases
   * Improved by using dilation, which adds a layer of pixels in an attempt to create more solid blob areas for tracking
* Global threshold inefficient for changing background or changing lighting as well as contour finding
   * Solved by allowing users to adjust threshold values
   * Also leads to better usability
* Having program capture first available frame for background and have it be unchangeable decreases usability and users have to restart the program for a new background.
   * Including options to relearn the background with a keypress or interface solves this problem
* Box2d shapes are not recognizing contour outlines
   * Solved by converting opencv blob outlines to box2d edges, which interact with ofxBox2dCircle
* Creating objects and interaction with contours quickly slowed program and caused crashes
   * Solved through catching bug of not deleting pointers in vectors before clearing and swapping with empty vectors
* Box2d objects can be created outside of box2d world, which was set to be the same size as the webcam image size
  * Solved by checking mouse coordinates on click

# Sources
**_Resources and documentation that were referenced while developing the program_**
* http://openframeworks.cc/ofBook/chapters/image_processing_computer_vision.html
* http://openframeworks.cc/documentation/ofxOpenCv/ofxCvColorImage/
* http://openframeworks.cc/documentation/video/ofVideoGrabber/
* http://openframeworks.cc/documentation/ofxOpenCv/ofxCvGrayscaleImage/
* http://openframeworks.cc/documentation/ofxOpenCv/ofxCvContourFinder/
* http://openframeworks.cc/documentation/graphics/ofPolyline/
* http://openframeworks.cc/documentation/ofxOpenCv/ofxCvBlob/
* http://box2d.org/manual.pdf and ofxBox2D examples
