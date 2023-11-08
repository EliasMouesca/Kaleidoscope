#include "kaleidoscope.h"

#include <assert.h>
#include <stdio.h>

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

bool doKaleidoscoping(SDL_Renderer* ren, SDL_Surface* srcSurface, SDL_Texture* dstTexture)
{
    if ( (ren == NULL) | (dstTexture == NULL) ) return false;

    SDL_Texture* oldTarget = SDL_GetRenderTarget(ren);

    int w = 0, h = 0;
    SDL_QueryTexture(dstTexture, NULL, NULL, &w, &h);
    SDL_Rect topLeftCorner = {0, 0, w / 2, h / 2};

    SDL_Texture* bufferTexture = SDL_CreateTexture(ren, 
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, w, h);
    if (bufferTexture == NULL) return false;
    
    SDL_Texture* cornerTexture = NULL;
    SDL_Surface* cornerSurface = SDL_CreateRGBSurface(
            0, w / 2, h / 2, 32, 
            R_MASK, G_MASK, B_MASK, A_MASK);
    SDL_SetSurfaceBlendMode(cornerSurface, SDL_BLENDMODE_NONE);

    // LOAD "cornerSurface" and mirror diagonally
    if (SDL_BlitSurface(srcSurface, NULL, cornerSurface, NULL) < 0) return false;
    //mirrorDiagonally(cornerSurface);

    // CONVERT "cornerSurface" TO TEXTURE
    {
        cornerTexture = SDL_CreateTextureFromSurface(ren, cornerSurface);
        SDL_FreeSurface(cornerSurface);
    }

    // MIRROR VERTICALLY AND HORIZONTALLY
    // Load cornerTexture to bufferTexture's up left corner
    SDL_SetRenderTarget(ren, bufferTexture);
    SDL_RenderCopy(ren, cornerTexture, NULL, &topLeftCorner);
    
    // Mirror vertically (bottom left)
    {
        SDL_Rect dst = {0, h / 2, w / 2, h / 2};
        SDL_RenderCopyEx(ren, cornerTexture, NULL, &dst, 
                0, NULL, SDL_FLIP_VERTICAL);
    }

    // Mirror horizontally (top right)
    {
        SDL_Rect dst = {w / 2, 0, w / 2, h / 2};
        SDL_RenderCopyEx(ren, cornerTexture, NULL, &dst,
                0, NULL, SDL_FLIP_HORIZONTAL);
    }

    // Mirror both (bottom right)
    {
        SDL_Rect dst = {w / 2, h / 2, w / 2, h / 2};
        SDL_RenderCopyEx(ren, cornerTexture, NULL, &dst,
                0, NULL, SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
    }

    SDL_DestroyTexture(cornerTexture);

    // Copy from bufferTexture to actual texture
    SDL_SetRenderTarget(ren, dstTexture);
    SDL_RenderCopy(ren, bufferTexture, NULL, NULL);

    SDL_DestroyTexture(bufferTexture);
    SDL_SetRenderTarget(ren, oldTarget);

    return true;
}

bool mirrorDiagonally(SDL_Surface* surface)
{
    SDL_LockSurface(surface);

    //void* auxPixels = calloc(surface->w * surface->h, sizeof(Uint32));
    //memcpy(auxPixels, surface->pixels, surface->w * surface->h);

    const double HEIGHT_OVER_WIDTH = (double) surface->h / (double) surface->w;
    //const double WIDTH_OVER_HEIGHT = (double) surface->w / (double) surface->h;

    for (int j = 0; j < surface->h; j++)
    for (int i = 0; i < surface->w; i++)
    {
        if ( (j < HEIGHT_OVER_WIDTH * i) )
        {
            *((Uint8*) surface->pixels + j * surface->pitch + i * surface->format->BytesPerPixel) =
                *((Uint8*) surface->pixels + (surface->h - j) * surface->pitch + (surface->w - i) * surface->format->BytesPerPixel);
        }

    }

    //free(auxPixels);
    SDL_UnlockSurface(surface);

    return true;
}

