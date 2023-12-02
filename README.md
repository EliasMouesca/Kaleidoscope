# Kaleidoscope
## What?
A kaleidoscope program! it does not reflect in hexagon shape (I didn't know how to do it, maybe in the future?), it mirrors a section jumping around the image diagonally, then vertically and horizontally. The image can be any image with height and width greater or equal than 600px, must be png or jpeg.

![image1](./img/readme1.png)
![image2](./img/readme2.png)

## Usage
By default the program runs windowed but flag "--fullscreen" makes it fullscreen. If you want to change the image it uses, just put an image in the "img" directory with the name "image" (no file extension) and the program will use that. Also, you can tweak the program by changing the "src/Constants.h" file, this requires recompiling (run make in the root folder).

## Why?
To entertain myself with something (though I should be studing...), and this way I don't have to search videos of kaleidoscopes in youtube and suffer the low resoution, framerate, and compression artifacts.

## Dependencies!
You should have SDL2 main and image library installed. The packages may be called libsdl2-dev and libsdl2-image-dev.

## Installation
just "make" should "install" (generate the binary and put it in the same diretory) it. It should compile and run on windows, did not test it.

