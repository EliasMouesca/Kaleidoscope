#include <SDL2/SDL_atomic.h>
#include <iostream>
#include <unistd.h>
#include "kaleidoscope.h"
#include <cassert>

bool check();

int main(int argc, char* argv[]) {
    if (!check()) return EXIT_FAILURE;

    Kaleidoscope k;
    assert(k.loadImage());

    while (!k.shutdown) {
        k.handleEvents();
        k.render();
    }

    return EXIT_SUCCESS;
}

bool check() {
    if( access( "img/image", F_OK ) != 0 ) {
        std::cerr << "Could not load image!\nIt seems the file 'image' is missing or is not an image..." << std::endl;
        return false;
    } 

    return true;
}


