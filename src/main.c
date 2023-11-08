#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include <assert.h>

#include "Constants.h"
#include "kaleidoscope.h"

int main()
{
    // Init
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG);

    SDL_Window* programWindow = SDL_CreateWindow(
            "DEMO", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            600, 600, WINDOW_FLAGS);
    SDL_Renderer* mainRenderer = SDL_CreateRenderer(programWindow, -1, 0);

    bool shutdown = false;
    Uint64 nextFrame = SDL_GetTicks64() + 1000 / WINDOW_FPS;

    SDL_Surface* imgSurface = IMG_Load("image.jpg");

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
            int windowWidth;
            int windowHeight;
            SDL_GetWindowSize(programWindow, &windowWidth, &windowHeight);

            SDL_Texture* canvasTexture = SDL_CreateTexture(mainRenderer, 
                    SDL_PIXELFORMAT_RGBA8888, 
                    SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight);

            //SDL_SetRenderTarget(mainRenderer, canvasTexture);

            //SDL_SetRenderDrawColor(mainRenderer, 200, 200, 134, 0xff);
            //SDL_RenderClear(mainRenderer);

            int a = doKaleidoscoping(mainRenderer, imgSurface, canvasTexture);
            if (a == false)
            {
                puts(SDL_GetError());
                return 1;
            }

            SDL_SetRenderTarget(mainRenderer, NULL);
            SDL_RenderCopy(mainRenderer, canvasTexture, NULL, NULL);
            SDL_RenderPresent(mainRenderer);
            SDL_DestroyTexture(canvasTexture);
        }
        else SDL_Delay(5);
    }

    SDL_FreeSurface(imgSurface);

    // Finish
    IMG_Quit();
    SDL_Quit();

    return 0;

}

