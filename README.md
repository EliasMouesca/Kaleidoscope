# Kaleidoscope
## What?
A kaleidoscope program! it does not reflect in hexagon shape (I didn't know how to do it, maybe in the future?), it mirrors a section jumping around the image diagonally, then vertically and horizontally. The image can be any image with height and width greater or equal than 600px, must be png or jpeg.

![image1](./img/readme1.png)
![image2](./img/readme2.png)

## Usage
If you want to change the image it uses, just put an image in the "img" directory with the name "image" (no file extension) and the program will use that.

## Why?
To entertain myself with something (though I should be studing...), and this way I don't have to search videos of kaleidoscopes in youtube and suffer the low resoution, framerate, and compression artifacts.

## Dependencies!
You should have SDL2 main and image library installed to compile the binary. The packages may be called libsdl2-dev and libsdl2-image-dev.

## Installation
just "make" should "install" (generate the binary and put it in the same directory) it. I do have the intention of making this available in mswindows, I don't plan do in it soon though.
Now, it's possible to run the program in a container but you should be on linux and use xorg.

## More info
This is actually Kaleidoscope V2. It's really similar to version 1 but this one calculates the next frame based on the time that has passed since the last update, doing it this way, it solved an artifact that would show up if the cpu got busy with something else, runining the smoothness of the program.


