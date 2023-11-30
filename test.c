#include "src/kaleidoscope.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <pthread.h>
#include <assert.h>

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

#define MIRROR_DIAGONALLY_TIMES 5000
#define DO_KALEIDOSCOPING_TIMES 10

void testMirrorDiagonally();
void* mirrorDiagonallyTestA(void* surface);
void* mirrorDiagonallyTestB(void* surface);

void testDoKaleidoscoping();
void* doKaleidoscopingTestA(void* args);
void* doKaleidoscopingTestB(void* args);

int main() 
{ 
    testDoKaleidoscoping();
    return 0;
}

void testDoKaleidoscoping()
{
    void* args = malloc(sizeof(void*) * 3);

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* win = SDL_CreateWindow("TEST", 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, 0);

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    SDL_Surface* surface = SDL_CreateRGBSurface(0, 300, 300, 32, R_MASK, G_MASK, B_MASK, A_MASK);

    SDL_Texture* texture = SDL_CreateTexture(ren, 
                SDL_PIXELFORMAT_RGBA8888, 
                SDL_TEXTUREACCESS_TARGET, 600, 600);

    ((void**) args)[0] = (void*) ren;
    ((void**) args)[1] = (void*) surface;
    ((void**) args)[2] = (void*) texture;

    assert(ren);
    assert(surface);
    assert(texture);

    pthread_t threadA, threadB;
    pthread_create(&threadA, NULL, &doKaleidoscopingTestA, args);
    pthread_create(&threadB, NULL, &doKaleidoscopingTestB, args);

    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    free(args);

    SDL_Quit();

    return;
}

void* doKaleidoscopingTestA(void* args)
{
    SDL_Renderer* ren = (SDL_Renderer*) ((void**) args)[0];
    SDL_Surface* surface = (SDL_Surface*) ((void**) args)[1];
    SDL_Texture* texture = (SDL_Texture*) ((void**) args)[2];

    assert(ren);
    assert(surface);
    assert(texture);

    clock_t before = clock();
    for (int i = 0; i < DO_KALEIDOSCOPING_TIMES; i++);
        doKaleidoscopingA(ren, surface, texture);
    clock_t after = clock();

    printf("Without: %lf", (double) (after - before) / DO_KALEIDOSCOPING_TIMES);
    
    return NULL;
}

void* doKaleidoscopingTestB(void* args)
{
    SDL_Renderer* ren = (SDL_Renderer*) ((void**) args)[0];
    SDL_Surface* surface = (SDL_Surface*) ((void**) args)[1];
    SDL_Texture* texture = (SDL_Texture*) ((void**) args)[2];

    clock_t before = clock();
    for (int i = 0; i < DO_KALEIDOSCOPING_TIMES; i++);
        doKaleidoscoping(ren, surface, texture);
    clock_t after = clock();

    printf("Optimized: %lf", (double) (after - before) / DO_KALEIDOSCOPING_TIMES);
    
    return NULL;
}

void testMirrorDiagonally()
{
    printf("Beginning mirrorDiagonally() tests...\n");

    SDL_Surface* surface = SDL_CreateRGBSurface(
        0, 1000, 1000, 32, 
        R_MASK, G_MASK, B_MASK, A_MASK);
    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);

    pthread_t threadA, threadB;
    pthread_create(&threadA, NULL, &mirrorDiagonallyTestA, surface);
    pthread_create(&threadB, NULL, &mirrorDiagonallyTestB, surface);

    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);

    return;
}

void* mirrorDiagonallyTestA(void* surface)
{
    clock_t before = clock();
    for (int i = 0; i < MIRROR_DIAGONALLY_TIMES; i++)
        mirrorDiagonallyA(surface);
    clock_t after = clock();
    printf("Without optimization: %lf CPU cicles.\n", (double) (after - before) / MIRROR_DIAGONALLY_TIMES);

    return NULL;
}

void* mirrorDiagonallyTestB(void* surface)
{
    clock_t before = clock();
    for (int i = 0; i < MIRROR_DIAGONALLY_TIMES; i++)
        mirrorDiagonally(surface);
    clock_t after = clock();

    printf("Optimized: %lf CPU cicles.\n", (double) (after - before) / MIRROR_DIAGONALLY_TIMES);

    return NULL;
}



