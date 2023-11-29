#include "kaleidoscope.h"

#include <assert.h>
#include <stdio.h>

extern int R_MASK;
extern int G_MASK;
extern int B_MASK;
extern int A_MASK;

typedef float floatType;

bool doKaleidoscopingA(SDL_Renderer* ren, SDL_Surface* srcSurface, SDL_Texture* dstTexture)
{
    if (ren == NULL) 
    {
        puts("render was null!\n");
        return false;
    }

    if (srcSurface == NULL) 
    {
        puts("surface was null!\n");
        return false;
    }

    if (dstTexture == NULL) 
    {
        puts("texture was null!\n");
        return false;
    }

    SDL_Texture* oldTarget = SDL_GetRenderTarget(ren);

    int w = 0, h = 0;
    SDL_QueryTexture(dstTexture, NULL, NULL, &w, &h);
    SDL_Rect topLeftCorner = {0, 0, w / 2, h / 2};

    SDL_Texture* bufferTexture = SDL_CreateTexture(ren, 
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, w, h);
    if (bufferTexture == NULL) 
    {
        puts("bufferTexture is NULL\n");
        return false;
    }
    
    int maxSize = (w < h) ? w : h;
    SDL_Texture* cornerTexture = NULL;
    SDL_Surface* cornerSurface = SDL_CreateRGBSurface(
            0, maxSize / 2, maxSize / 2, 32, 
            R_MASK, G_MASK, B_MASK, A_MASK);
    SDL_SetSurfaceBlendMode(cornerSurface, SDL_BLENDMODE_NONE);

    // LOAD "cornerSurface" and mirror diagonally
    if (SDL_BlitSurface(srcSurface, NULL, cornerSurface, NULL) < 0)
    {
        puts("SDL_BlitSurface failed! \n");
        return false;
    }

    if (mirrorDiagonally(cornerSurface) == false)
    {
        puts("Diagonal mirror failed!\n");
    }

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

bool doKaleidoscoping(SDL_Renderer* ren, SDL_Surface* srcSurface, SDL_Texture* dstTexture)
{
    if (ren == NULL) 
    {
        puts("render was null!\n");
        return false;
    }

    if (srcSurface == NULL) 
    {
        puts("surface was null!\n");
        return false;
    }

    if (dstTexture == NULL) 
    {
        puts("texture was null!\n");
        return false;
    }

    SDL_Texture* oldTarget = SDL_GetRenderTarget(ren);

    int w = 0, h = 0;
    SDL_QueryTexture(dstTexture, NULL, NULL, &w, &h);
    SDL_Rect topLeftCorner = {0, 0, w / 2, h / 2};

    SDL_Texture* bufferTexture = SDL_CreateTexture(ren, 
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, w, h);
    if (bufferTexture == NULL) 
    {
        puts("bufferTexture is NULL\n");
        return false;
    }
    
    int maxSize = (w < h) ? w : h;
    SDL_Texture* cornerTexture = NULL;
    SDL_Surface* cornerSurface = SDL_CreateRGBSurface(
            0, maxSize / 2, maxSize / 2, 32, 
            R_MASK, G_MASK, B_MASK, A_MASK);
    SDL_SetSurfaceBlendMode(cornerSurface, SDL_BLENDMODE_NONE);

    // LOAD "cornerSurface" and mirror diagonally
    if (SDL_BlitSurface(srcSurface, NULL, cornerSurface, NULL) < 0)
    {
        puts("SDL_BlitSurface failed! \n");
        return false;
    }

    if (mirrorDiagonally(cornerSurface) == false)
    {
        puts("Diagonal mirror failed!\n");
    }

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

// This was the fastest implementation I could write, don't touch it unless measuring performance!
bool mirrorDiagonally(SDL_Surface* surface)
{
    if (surface->w != surface->h) return false;

    SDL_LockSurface(surface);

    for (int j = 0; j < surface->h; j++)
    for (int i = j; i < surface->w; i++)
    {
        Uint32* srcPixel = (Uint32*) surface->pixels + i * surface->w + j;
        Uint32* dstPixel = (Uint32*) surface->pixels + j * surface->w + i;

        *dstPixel = *srcPixel;
    }

    SDL_UnlockSurface(surface);

    return true;
}
