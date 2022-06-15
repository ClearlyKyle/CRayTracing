#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "SDL2/SDL.h"

struct Image_s
{
    double *r;
    double *g;
    double *b;

    unsigned int x_size;
    unsigned int y_size;

    SDL_Renderer *renderer;
    SDL_Texture  *texture;
};

typedef struct Image_s Image_t;

Image_t Image_Initialize(const int x_size, const int y_size, SDL_Renderer *pRenderer);

void Image_SetPixel(Image_t *image, const int x, const int y, const double red, const double green, const double blue);
void Image_Display(const Image_t *const image);

Uint32 ConvertColor(const double red, const double green, const double blue);
void   InitTexture();

#endif // __IMAGE_H__