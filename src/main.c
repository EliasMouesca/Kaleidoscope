#include <SDL2/SDL.h>
#include <stdbool.h>

int main()
{
    // Init
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPEG);

    bool shutdown = false;
    Uint64 nextFram

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

        doKaleidoscoping(
    }


    // Finish
    IMG_Quit();
    SDL_Quit();



