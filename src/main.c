#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

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

int main()
{
    // Init
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG);

    SDL_Window* programWindow = SDL_CreateWindow(
            "DEMO", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS);
    SDL_Renderer* mainRenderer = SDL_CreateRenderer(programWindow, -1, 0);

    bool shutdown = false;
    Uint64 nextFrame = SDL_GetTicks64() + 1000 / WINDOW_FPS;

    SDL_Surface* imgSurface = IMG_Load("image.jpg");
    Uint32 msCounter = 0;
    const Uint32 MS_COUNTER_PERIOD = 10000; // Every 10'000 seconds it will reset
    Uint32 lastTick = SDL_GetTicks();


    while (!shutdown)
    {
        SDL_Event eventPoll;

        SDL_PollEvent(&eventPoll);

        if (eventPoll.type == SDL_QUIT)
        {
            shutdown = true;
        }

        // Render
        if (SDL_GetTicks64() >= nextFrame)
        {
            nextFrame = SDL_GetTicks64() + 1000 / WINDOW_FPS;
            int windowWidth;
            int windowHeight;
            SDL_GetWindowSize(programWindow, &windowWidth, &windowHeight);

            SDL_Surface* movingSurface = SDL_CreateRGBSurface(0, windowWidth / 2, windowHeight / 2, 32, R_MASK, G_MASK, B_MASK, A_MASK);

            SDL_Texture* canvasTexture = SDL_CreateTexture(mainRenderer, 
                    SDL_PIXELFORMAT_RGBA8888, 
                    SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight);

            SDL_Rect position = {(msCounter / 10000.0) * imgSurface->w, 0, windowWidth / 2, windowHeight / 2};
            SDL_BlitSurface(imgSurface, &position, movingSurface, NULL);

            if (doKaleidoscoping(mainRenderer, movingSurface, canvasTexture, msCounter / 1000.0) == false)
            {
                printf("doKaleidoscoping failed! error: %s\n", SDL_GetError());
                return 1;
            }

            SDL_SetRenderTarget(mainRenderer, NULL);
            SDL_RenderCopyEx(mainRenderer, canvasTexture, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
            SDL_RenderPresent(mainRenderer);

            SDL_DestroyTexture(canvasTexture);
            SDL_FreeSurface(movingSurface);

            msCounter += SDL_GetTicks() - lastTick;
            lastTick = SDL_GetTicks();
            if (msCounter >= MS_COUNTER_PERIOD) msCounter = 0;

        }
        else SDL_Delay(5);
    }

    SDL_FreeSurface(imgSurface);

    // Finish
    IMG_Quit();
    SDL_Quit();

    return 0;

}

