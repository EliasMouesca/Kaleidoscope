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
#include <time.h>

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
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    
    // Main variables
    SDL_Window* programWindow = NULL;
    SDL_Renderer* mainRenderer = NULL;
    SDL_Surface* imgSurface = NULL;
    int windowWidth;
    int windowHeight;
    bool shutdown = false;
    int retValue = 0;
    floatType x = 0, y = 0, dx = 0, dy = 0;
    int X_UPPER_BOUND = 0;
    int Y_UPPER_BOUND = 0;

    // Manage Arguments and create window
    {
        bool fullscreen = false;
        int flags = WINDOW_FLAGS;

        if (argc > 1)
        {
            if (strcmp(argv[1], "--fullscreen") == 0) fullscreen = true;
            else if (strcmp(argv[1], "--windowed") == 0) fullscreen = false;
        }

        if (fullscreen) 
        {
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
            SDL_ShowCursor(SDL_DISABLE);
        }

        programWindow = SDL_CreateWindow(
            WINDOW_TITLE, 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            WINDOW_WIDTH, WINDOW_HEIGHT, flags);
    }

    mainRenderer = SDL_CreateRenderer(programWindow, -1, 0);

    // Set icon
    {
        SDL_Surface* iconSurface = IMG_Load(ICON_PATH);
        if (iconSurface == NULL) puts("Could not load icon.png");
        else
        {
            SDL_SetWindowIcon(programWindow, iconSurface);
            SDL_FreeSurface(iconSurface);
        }
    }

    imgSurface = IMG_Load(IMAGE_PATH);
    if (imgSurface == NULL) 
    {
        puts("Could not load 'image' file. Maybe it isn't jpeg or png?");
        retValue = 1;
        goto exit;
    }

    if ( (imgSurface->w < 600) | (imgSurface->w < 600) )
    {
        puts("Images must be at least 600x600");
        SDL_FreeSurface(imgSurface);
        retValue = 1;
        goto exit;
    }
    
    SDL_SetWindowMaximumSize(programWindow, imgSurface->w, imgSurface->h);

    srand(time(NULL));

    SDL_GetWindowSize(programWindow, &windowWidth, &windowHeight);
    x = rand() % (imgSurface->w - windowWidth / 2),     // x value for input from img
    y = rand() % (imgSurface->h - windowHeight / 2),    // y value for input from img 
    dx = DX,                                            // Tiny nudge added to x every frame
    dy = DY;                                            // Idem dx for y
    X_UPPER_BOUND = imgSurface->w;
    Y_UPPER_BOUND = imgSurface->h;

    while (!shutdown) 
    {
        clock_t firstTick = clock();

        SDL_Event eventPoll;
        SDL_PollEvent(&eventPoll);
        if (eventPoll.type == SDL_QUIT) shutdown = true;

        // Calculate x and y for input image
        SDL_GetWindowSize(programWindow, &windowWidth, &windowHeight);
        x += dx;
        y += dy;

        if (x + windowWidth / 2 >= X_UPPER_BOUND)
        {
            x = X_UPPER_BOUND - windowWidth / 2 - 1;
            dx *= -1.0f;
        }

        if (x < 0)
        {
            x = 0;
            dx *= -1.0f;
        }

        if (y + windowHeight / 2 >= Y_UPPER_BOUND)
        {
            y = Y_UPPER_BOUND - windowHeight / 2 - 1;
            dy *= -1.0f;
        }

        if (y < 0)
        {
            y = 0;
            dy *= -1.0f;
        }

        SDL_GetWindowSize(programWindow, &windowWidth, &windowHeight);

        // This suruface will be jumping around the image with a fix size
        SDL_Surface* movingSurface = SDL_CreateRGBSurface(0, windowWidth / 2, windowHeight / 2, 32, R_MASK, G_MASK, B_MASK, A_MASK);

        // This texture will eventually be rendered to screen
        SDL_Texture* canvasTexture = SDL_CreateTexture(mainRenderer, 
                SDL_PIXELFORMAT_RGBA8888, 
                SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight);


        // Copy from the imgSurface to the movingSurface
        SDL_Rect position = {(int) (x + 0.5f), (int) (y + 0.5f), windowWidth / 2, windowHeight / 2};
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

        clock_t currentTick = clock();
        double timePassed = (double) (currentTick - firstTick) * 1000.0 / CLOCKS_PER_SEC;
        static int FRAMES_SINCE_LAST_OVERFLOW = 0;
        static int OVERFLOW_COUNTER = 0;
        if (timePassed > DELTA_TIME)
        {
            FRAMES_SINCE_LAST_OVERFLOW = 0;
            if ( (++OVERFLOW_COUNTER > OVERFLOW_TOLERANCE) && (SDL_GetTicks() > INITIALIZATION_TIME) )
            {

                printf("The time between frames surpassed the delta time expected to mantain the FPS rate (1000 / FPS = %lfms) several times => FPS should be lowered! (recompiling)\n", DELTA_TIME);
                retValue = 1;
                goto exit;
            }
        }
        else 
        {
            if (++FRAMES_SINCE_LAST_OVERFLOW > OVERFLOW_WINDOW) OVERFLOW_COUNTER = 0;
            SDL_Delay(DELTA_TIME - timePassed);
        }

    } // End while(!shutdown)

    SDL_FreeSurface(imgSurface);

    // Finish
exit:
    SDL_DestroyRenderer(mainRenderer);
    SDL_DestroyWindow(programWindow);

    IMG_Quit();
    SDL_Quit();

    return retValue;

}

