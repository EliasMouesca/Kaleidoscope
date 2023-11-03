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
            600, 600, 0);
    SDL_Renderer* mainRenderer = SDL_CreateRenderer(programWindow, -1, 0);

    bool shutdown = false;
    Uint64 nextFrame = SDL_GetTicks64() + 1000 / WINDOW_FPS;

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
            SDL_Surface* sur = IMG_Load("image.jpg");
            SDL_Texture* tex = SDL_CreateTextureFromSurface(mainRenderer, sur);

            SDL_RenderCopy(mainRenderer, tex, NULL, NULL);

            //doKaleidoscoping(mainRenderer, 

            SDL_RenderPresent(mainRenderer);
        }
    }


    // Finish
    IMG_Quit();
    SDL_Quit();

    return 0;

}

