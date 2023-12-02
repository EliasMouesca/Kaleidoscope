#include <SDL2/SDL.h>

typedef double floatType;

const char WINDOW_TITLE[] = "Kaleidoscope";

// These regulate how tolerant is the program to skipping frames
const int OVERFLOW_WINDOW = 60;                 
const int OVERFLOW_TOLERANCE = 5;
// If in the last <OVERFLOW_WINDOW> frames there were more than <OVERFLOW_TOLERANCE> overflows, shutdown the program.

// This is the number of miliseconds the program waits before starting to measure performance and crash.
// This is so that it doesn't crash because it spend sometime setting fullscreen, loading the image, etc. 
const int INITIALIZATION_TIME = 5000;


// These are the variables you _should_ change. If you want, change everything but this program is not that stable.
const int WINDOW_FPS = 120;
const int WINDOW_WIDTH = 600;   // Then you can resize the window, but by default, it uses these values.
const int WINDOW_HEIGHT = 600;
// This 2 variables should depend on FPS so that when WINDOW_FPS constant is changed, this ones change acordingly
const floatType DX = (floatType) 100.0 / WINDOW_FPS; // Tiny nudge added to x every frame
const floatType DY = (floatType) 100.0 / WINDOW_FPS; // Ídem DX

                                                     
const floatType DELTA_TIME = 1000.0 / WINDOW_FPS;
const Uint32 WINDOW_FLAGS = SDL_WINDOW_RESIZABLE;

const char ICON_PATH[] = "./img/icon.png";
const char IMAGE_PATH[] = "./img/image";

