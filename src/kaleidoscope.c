#include "kaleidoscope.h"

#include <assert.h>
#include <stdio.h>

void doKaleidoscoping(SDL_Renderer* ren, SDL_Texture* texture)
{
    SDL_Texture* oldTarget = SDL_GetRenderTarget(ren);

    int w = 0, h = 0;

    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    SDL_Texture* auxiliarTexture = SDL_CreateTexture(ren, 
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, w, h);
    
    SDL_SetRenderTarget(ren, auxiliarTexture);

    // Load top left corner from texture to auxiliarTexture
    {
        SDL_Rect src = {0, 0, w / 2, h / 2};

        SDL_RenderCopy(ren, texture, &src, &src);
    }
    
    // Mirror vertically
    {
        SDL_Rect srcVertical = {0, 0, w / 2, h / 2};
        SDL_Rect dstVertical = {0, h / 2, w / 2, h / 2};
        SDL_RenderCopyEx(ren, texture, &srcVertical, &dstVertical,
                0, NULL, SDL_FLIP_VERTICAL);
    }

    // Mirror horizontally
    {
        SDL_Rect srcHorizontal = {0, 0, w / 2, h};
        SDL_Rect dstHorizontal = {w / 2, 0, w / 2, h};
        SDL_RenderCopyEx(ren, texture, &srcHorizontal, &dstHorizontal,
                0, NULL, SDL_FLIP_HORIZONTAL);
    }

    SDL_SetRenderTarget(ren, texture);
    SDL_RenderCopy(ren, auxiliarTexture, NULL, NULL);
    SDL_DestroyTexture(auxiliarTexture);

    SDL_SetRenderTarget(ren, oldTarget);

    return;
}

