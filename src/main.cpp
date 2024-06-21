#include <SDL2/SDL_atomic.h>
#include <iostream>
#include "kaleidoscope.h"


int main(int argc, char* argv[])
{
    Kaleidoscope k;
    
    if (k.loadImage() == false) 
    {
        puts("Could not load the image!");
        puts("It seems that 'image' file is missing or is not an image...");

        return 1;
    }

    while (!k.shutdown)
    {
        k.handleEvents();
        k.render();
    }

    return 0;
}
