#ifdef WIN32
    #include <SDL.h>
    #include <SDL_image.h>
#else
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <assert.h>

#include "Constants.h"
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

int main(int argc, char* argv[])
{
    // Arguments
    bool fullscreen = false;
    int flags = WINDOW_FLAGS;

    if (argc > 1)
    {
        if (strcmp(argv[1], "--fullscreen") == 0) fullscreen = true;
        else if (strcmp(argv[1], "--windowed") == 0) fullscreen = false;
    }

    if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    // Init
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    SDL_Window* programWindow = SDL_CreateWindow(
            WINDOW_TITLE, 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            WINDOW_WIDTH, WINDOW_HEIGHT, flags);

    SDL_Renderer* mainRenderer = SDL_CreateRenderer(programWindow, -1, 0);

    // Set icon
    SDL_Surface* iconSurface = IMG_Load("icon.png");
    SDL_SetWindowIcon(programWindow, iconSurface);
    SDL_FreeSurface(iconSurface);

    // Variables for the loop
    bool shutdown = false;
    Uint64 nextFrame = SDL_GetTicks64() + 1000 / WINDOW_FPS;

    SDL_Surface* imgSurface = IMG_Load("image");

    srand(time(NULL));

    double  x = rand() % (imgSurface->w - WINDOW_WIDTH / 2),    // x value for input from img
            y = rand() % (imgSurface->h - WINDOW_HEIGHT / 2),   // y value for input from img 
            dx = DX,     // Tiny nudge added to x every frame
            dy = DY;     // Idem dx for y
    const int X_UPPER_BOUND = imgSurface->w;
    const int Y_UPPER_BOUND = imgSurface->h;

    while (!shutdown)
    {
        SDL_Event eventPoll;

        SDL_PollEvent(&eventPoll);

        if (eventPoll.type == SDL_QUIT) shutdown = true;

        if (SDL_GetTicks64() >= nextFrame)
        {
            nextFrame = SDL_GetTicks64() + 1000 / WINDOW_FPS;
            int windowWidth;
            int windowHeight;
            SDL_GetWindowSize(programWindow, &windowWidth, &windowHeight);

            // This suruface will be jumping around the image with a fix size
            SDL_Surface* movingSurface = SDL_CreateRGBSurface(0, windowWidth / 2, windowHeight / 2, 32, R_MASK, G_MASK, B_MASK, A_MASK);

            // This texture will eventually be rendered to screen
            SDL_Texture* canvasTexture = SDL_CreateTexture(mainRenderer, 
                    SDL_PIXELFORMAT_RGBA8888, 
                    SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight);

            // Calculate x and y for input image
            x += dx;
            y += dy;

            if (x + windowWidth / 2 >= X_UPPER_BOUND)
            {
                x = X_UPPER_BOUND - windowWidth / 2 - 1;
                dx *= -1;
            }

            if (x < 0)
            {
                x = 0;
                dx *= -1;
            }

            if (y + windowHeight / 2 >= Y_UPPER_BOUND)
            {
                y = Y_UPPER_BOUND - windowHeight / 2 - 1;
                dy *= -1;
            }

            if (y < 0)
            {
                y = 0;
                dy *= -1;
            }

            // Copy from the imgSurface to the movingSurface
            SDL_Rect position = {x, y, windowWidth / 2, windowHeight / 2};
            SDL_BlitSurface(imgSurface, &position, movingSurface, NULL);

            // Take the render and the input in movingSurface, do kaleidoscope and put it in the canvasTexture
            if (doKaleidoscoping(mainRenderer, movingSurface, canvasTexture) == false)
            {
                printf("doKaleidoscoping failed! error: %s\n", SDL_GetError());
                return 1;
            }

            // Render the canvasTexture to the screen
            SDL_SetRenderTarget(mainRenderer, NULL);
            SDL_RenderCopy(mainRenderer, canvasTexture, NULL, NULL);
            SDL_RenderPresent(mainRenderer);

            SDL_DestroyTexture(canvasTexture);
            SDL_FreeSurface(movingSurface);

        }
        else SDL_Delay(5);
    }

    SDL_FreeSurface(imgSurface);

    // Finish
    IMG_Quit();
    SDL_Quit();

    return 0;

}

