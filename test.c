#include "src/kaleidoscope.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <pthread.h>

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

#define times 10000

void* testA(void* surface)
{
    clock_t before = clock();
    for (int i = 0; i < times; i++)
        mirrorDiagonallyA(surface);
    clock_t after = clock();
    printf("Without optimization: %lf CPU cicles.\n", (double) (after - before) / times);

    return NULL;
}

void* testB(void* surface)
{
    clock_t before = clock();
    for (int i = 0; i < times; i++)
        mirrorDiagonally(surface);
    clock_t after = clock();

    printf("Optimized: %lf CPU cicles.\n", (double) (after - before) / times);

    return NULL;
}

int main()
{
    printf("Beginning mirrorDiagonally() tests...\n");

    SDL_Surface* surface = SDL_CreateRGBSurface(
            0, 1000, 1000, 32, 
            R_MASK, G_MASK, B_MASK, A_MASK);
    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);

    pthread_t threadA, threadB;
    pthread_create(&threadA, NULL, &testA, surface);
    pthread_create(&threadB, NULL, &testB, surface);

    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);

    return 0;
}




