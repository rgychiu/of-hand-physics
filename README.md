# OF-Hand-Physics
This project involves an application of computer vision and image processing, specifically with OpenCv hand tracking and Box2D, a 2D physics game engine. It allows for object tracking with any background using live video from a webcam, which is accomplished using background subtraction with one channel images in combination with contour finding and blob detection. In addition, objects can be created that collide and interact with the tracked bodies within the displayed webcam feed.

## Installing
### Prerequisites
This project uses OpenFrameworks and some of its addons. If not already installed, download the latest release here:
```
http://openframeworks.cc/download/
```
In addition, this project uses Box2D, an extra 2D physics game engine addon that is not listed under the included addons with OpenFrameworks. If not already downloaded, it can be acquired through following the instructions at this repo:
```
https://github.com/vanderlin/ofxBox2d
```
Follow the installation instructions and make sure that the installed folder is in the addons folder of the OpenFrameworks directory.

### Setup
First, clone the repository:
```
https://github.com/rgychiu/of-hand-physics.git
```
Copy the cloned repository into the `apps/myApps` folder of the OpenFrameworks directory.

Open the Project Generator from the `projectGenerator` folder of the OpenFrameworks directory and select `import`.

Navigate to and select the cloned repository in the myApps folder and update the project. Make sure OpenCv and Box2D are recognized addons and included in the project.

Open the project in your IDE, and you should now be able to build and run the project!

## Usage
The project includes a basic interface that lists the different commands available when running. Nonetheless, here are some steps to use the features of the project:

When the program runs, a video along with a black and white video will appear in the window. The black and white image is the result of background subtraction between a grayscaled version of the live video and a background image, which defaults to the first frame that was available when the program started and webcam turned on.

To use another background for background subtraction, position the webcam to include the new desired background and press `R`.

The program will continue to attempt to highlight blobs present in the black and white video. If objects are not recognized, decreasing the threshold using `[` or increasing the threshold using `]` can help with differentiating the background from other objects that move into the frame.

Even without any bodies being tracked, Box2D shapes can be created and will be contained within the video. To create a circle object, simply click anywhere in the video. To clear all the objects from the video, press `-`

**Note that it is possible that your computer's webcam does not support 640x480. If this is the case, you may need to adjust the size of the window. Otherwise, the layout should adjust based on the new width and height.**
