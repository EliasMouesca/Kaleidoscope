#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <cassert>
#include <stdlib.h>
#include <time.h>

#include "kaleidoscope.h"

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

#define BASE_IMAGE "img/image"

bool mirrorDiagonally(SDL_Surface* surface);

Kaleidoscope::Kaleidoscope() { 
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Could not initialize SDL_VIDEO" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0) {
        std::cerr << "Could not initialize SDL_IMG" << std::endl;
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), 
            0, 0, 300, 300, 
            SDL_WINDOW_RESIZABLE);

    if (window == NULL) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        return;
    }


    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        return;
    }

    SDL_ShowCursor(SDL_DISABLE);
    
    srand(time(NULL));

    return;

}

Kaleidoscope::~Kaleidoscope() {
    if (imageSurface) SDL_FreeSurface(imageSurface);
    if (imageTexture) SDL_DestroyTexture(imageTexture);

    if (canvasTexture) SDL_DestroyTexture(canvasTexture);
    if (lastCanvasTexture) SDL_DestroyTexture(lastCanvasTexture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return;
}

bool Kaleidoscope::loadImage() {
    if (imageSurface != NULL) return false;

    if ( (imageSurface = IMG_Load(BASE_IMAGE)) == NULL ) return false;
    imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    pictureWidth = imageSurface->w;
    pictureHeight = imageSurface->h;

    canvasWidth = pictureWidth / SCALE_FACTOR;
    canvasHeight = pictureHeight / SCALE_FACTOR;

    canvasTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, canvasWidth, canvasHeight);
    lastCanvasTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, canvasWidth, canvasHeight);

    return true;
}

bool Kaleidoscope::render() {

    SDL_Point newPosition;
    static SDL_Point lastPosition;

    static Uint64 previousTick = 0;
    Uint64 currentTick = SDL_GetTicks64();
    Uint64 diff = currentTick - previousTick;

    // Maybe last update was too quick, take a breath...
    // This is in case you have a NASA computer, so that it doesn't consume too much CPU
    if (diff < 1) {
        // If you want 60 FPS, this is only 1/16 of the delay you should wait. But the OS garantees you
        // wait _more_ than <delay> ms, never less, so just in case.
        previousTick = currentTick;
        SDL_Delay(diff);
        return false;
    }

    // Calculate next position for the square
    getNextPosition(SDL_GetTicks64(), newPosition);

    const int xOffset = lastPosition.x - newPosition.x;
    const int yOffset = lastPosition.y - newPosition.y;

    // Maybe the increment was too small to actually render again
    if ( (xOffset == 0) && (yOffset == 0) ) return false;


    // == BEGIN RENDERING ==

    SDL_Rect imageCoords = {newPosition.x, newPosition.y, canvasWidth, canvasHeight};
    SDL_Surface* bufferSurface = SDL_CreateRGBSurface(0, std::max(windowWidth, windowHeight) / 2, std::max(windowWidth, windowHeight) / 2, 32, R_MASK, G_MASK, B_MASK, A_MASK);

    SDL_SetRenderTarget(renderer, canvasTexture);
    SDL_BlitSurface(imageSurface, &imageCoords, bufferSurface, NULL);

    mirrorDiagonally(bufferSurface);

    SDL_Texture* bufferTexture = SDL_CreateTextureFromSurface(renderer, bufferSurface);

    SDL_RenderCopy(renderer, bufferTexture, NULL, NULL);

    SDL_DestroyTexture(bufferTexture);

    SDL_RenderPresent(renderer);

    SDL_FreeSurface(bufferSurface);

    SDL_SetRenderTarget(renderer, lastCanvasTexture);
    SDL_RenderCopy(renderer, canvasTexture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Rect topLeftCorner = {0, 0, windowWidth / 2, windowHeight / 2};

    // Render canvasTexture to topLeftCorner of screen
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, canvasTexture, NULL, &topLeftCorner);

    int w = windowWidth, h = windowHeight;

    // Mirror vertically (bottom left)
    {
        SDL_Rect destination = {0, h / 2, w / 2, h / 2};
        SDL_RenderCopyEx(renderer, canvasTexture, NULL, &destination, 0, NULL, SDL_FLIP_VERTICAL);
    }

    // Mirror horizontally (top right)
    {
        SDL_Rect destination = {w / 2, 0, w / 2, h / 2};
        SDL_RenderCopyEx(renderer, canvasTexture, NULL, &destination, 0, NULL, SDL_FLIP_HORIZONTAL);
    }

    // Mirror both (bottom right)
    {
        SDL_Rect destination = {w / 2, h / 2, w / 2, h / 2};
        SDL_RendererFlip flip = (SDL_RendererFlip) (SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
        SDL_RenderCopyEx(renderer, canvasTexture, NULL, &destination, 0, NULL, flip);
    }

    SDL_RenderPresent(renderer);

    lastPosition = newPosition;
    previousTick = currentTick;

    return true;
}

bool mirrorDiagonally(SDL_Surface* surface) {
    if (surface->w != surface->h) return false;

    SDL_LockSurface(surface);

    int width = surface->w;
    int height = surface->h;
    Uint32* pixels = (Uint32*) surface->pixels;

    for (int j = 0; j < height; j++) {
        for (int i = j + 1; i < width; i++) {
            Uint32* srcPixel = pixels + i * width + j;
            Uint32* dstPixel = pixels + j * width + i;

            *dstPixel = *srcPixel;
        }
    }

    SDL_UnlockSurface(surface);

    return true;
}



bool Kaleidoscope::handleEvents()
{
    SDL_Event event;

    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    while (SDL_PollEvent(&event))
        if (event.type == SDL_QUIT) shutdown = true;

    return true;
}

bool Kaleidoscope::getNextPosition(Uint64 currentTick, SDL_Point &point)
{
    int limitX = pictureWidth - canvasWidth;
    int limitY = pictureHeight - canvasHeight;

    static double xVel = X_VELOCITY;
    static double yVel = Y_VELOCITY;

    static Uint64 lastTick = 0;
    Uint64 deltaTime = currentTick - lastTick;
    lastTick = currentTick;

    static double x = rand() % limitX, y = rand() % limitY;
    int nextX = x + deltaTime * xVel;
    int nextY = y + deltaTime * yVel;

    // If the next step is out of bounds, invert the velocities
    if ( (nextX > limitX) || (nextX < 0) ) xVel *= -1.0;
    if ( (nextY > limitY) || (nextY < 0) ) yVel *= -1.0;

    x += deltaTime * xVel;
    y += deltaTime * yVel;
    
    // Something happened, return random values
    if (x < 0) x = rand() % limitX;
    if (y < 0) y = rand() % limitY;

    point.x = x;
    point.y = y;

    return true;
}

