#include "src/kaleidoscope.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const uint32_t R_MASK = 0xff000000;
    const uint32_t G_MASK = 0x00ff0000;
    const uint32_t B_MASK = 0x0000ff00;
    const uint32_t A_MASK = 0x000000ff;
#else
    const uint32_t R_MASK = 0x000000ff;
    const uint32_t G_MASK = 0x0000ff00;
    const uint32_t B_MASK = 0x00ff0000;
    const uint32_t A_MASK = 0xff000000;
#endif

const int times = 10000;

int main()
{
    clock_t before, after;
    printf("Beginning mirrorDiagonally() tests...\n");

    SDL_Surface* cornerSurface = SDL_CreateRGBSurface(
            0, 600, 600, 32, 
            R_MASK, G_MASK, B_MASK, A_MASK);
    SDL_SetSurfaceBlendMode(cornerSurface, SDL_BLENDMODE_NONE);

    before = clock();
    for (int i = 0; i < times; i++)
        mirrorDiagonallyA(cornerSurface);
    after = clock();
    printf("Without optimization: %lf CPU cicles.\n", (double) (after - before) / times);

    before = clock();
    for (int i = 0; i < times; i++)
        mirrorDiagonally(cornerSurface);
    after = clock();

    printf("Optimized: %lf CPU cicles.\n", (double) (after - before) / times);

    return 0;
}




