#include <SDL2/SDL.h>

const char WINDOW_TITLE[] = "Kaleidoscope";
const int WINDOW_FPS = 120;
const double DELTA_TIME = 1000.0 / WINDOW_FPS;
const Uint32 WINDOW_FLAGS = SDL_WINDOW_RESIZABLE;
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

// This 2 variables should depend on FPS so that when WINDOW_FPS constante is changed, this one is changed acordingly
const double DX = (double) 100 / WINDOW_FPS; // Tiny nudge added to x every frame
const double DY = (double) 100 / WINDOW_FPS; // Ídem DX

const char ICON_PATH[] = "./img/icon.png";
const char IMAGE_PATH[] = "./img/image";

