#include "kaleidoscope.h"

void doKaleidoscoping(SDL_Renderer* ren, SDL_Texture* texture)
{
    SDL_Texture* oldTarget = SDL_GetRenderTarget(ren);
    
    
    SDL_SetRenderTarget(ren, texture);

    SDL_Point size;
    SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
    
    // Mirror vertically
    {
        SDL_Rect srcVertical = {0, 0, 
            size.x / 2, size.y / 2};
        SDL_Rect dstVertical = {0, size.y / 2, 
            size.x / 2, size.y / 2};
        SDL_RenderCopyEx(ren, texture, &srcVertical, &dstVertical,
                0, NULL, SDL_FLIP_VERTICAL);
    }

    // Mirror horizontally
    {
        SDL_Rect srcHorizontal = {0, 0, 
            size.x / 2, size.y};
        SDL_Rect dstHorizontal = {size.x / 2, 0, 
            size.x / 2, size.y};
        SDL_RenderCopyEx(ren, texture, &srcHorizontal, &dstHorizontal,
                0, NULL, SDL_FLIP_HORIZONTAL);
    }

    SDL_SetRenderTarget(ren, oldTarget);

    return;
}

