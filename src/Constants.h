#include <SDL2/SDL.h>

const char WINDOW_TITLE[] = "Kaleidoscope";
const int WINDOW_FPS = 120;
const double DELTA_TIME = 1000.0 / WINDOW_FPS;
const Uint32 WINDOW_FLAGS = SDL_WINDOW_RESIZABLE;
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

const double DX = 0.7; // Tiny nudge added to x every frame
const double DY = 0.7; // Ídem DX

const char ICON_PATH[] = "./img/icon.png";
const char IMAGE_PATH[] = "./img/image";

