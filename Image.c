#include "Image.h"

Image_t Image_Initialize(const int x_size, const int y_size, SDL_Renderer *renderer)
{
    Image_t image;

    image.r = (double *)calloc(y_size * x_size, sizeof(double));
    image.g = (double *)calloc(y_size * x_size, sizeof(double));
    image.b = (double *)calloc(y_size * x_size, sizeof(double));

    image.x_size = x_size;
    image.y_size = y_size;

    image.renderer = renderer;

    SDL_Surface *tmp_surface;
    tmp_surface = SDL_CreateRGBSurfaceWithFormat(0, image.x_size, image.y_size, 32, SDL_PIXELFORMAT_ABGR8888);
    if (tmp_surface == NULL)
    {
        fprintf(stderr, "SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError());
        exit(1);
    }
    image.texture = SDL_CreateTextureFromSurface(image.renderer, tmp_surface);
    SDL_FreeSurface(tmp_surface);

    return image;
}

void Image_SetPixel(Image_t *image, const int x, const int y, const double red, const double green, const double blue)
{
    const int index = y * image->x_size + x;

    image->r[index] = red;
    image->g[index] = green;
    image->b[index] = blue;
}

void Image_Display(const Image_t *const image)
{
    // Allocate memory for a pixel buffer
    Uint32 *tmp_pixels = (Uint32 *)malloc(sizeof(Uint32) * (image->x_size * image->y_size));

    // Clear the pixel buffer
    memset(tmp_pixels, 0, sizeof(Uint32) * (image->x_size * image->y_size));

    for (unsigned x = 0; x < image->x_size; ++x)
    {
        for (unsigned y = 0; y < image->y_size; ++y)
        {
            const unsigned index = y * image->x_size + x;

            tmp_pixels[(y * image->x_size) + x] = ConvertColor(image->r[index], image->g[index], image->b[index]);
        }
    }

    // Update the texture with the pixel buffer
    SDL_UpdateTexture(image->texture, NULL, tmp_pixels, image->x_size * sizeof(Uint32));

    // Destroy the pixel buffer.
    free(tmp_pixels);
    tmp_pixels = NULL;

    // Copy the texture to the renderer.
    SDL_Rect srcRect;
    // SDL_Rect bounds;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = 800;
    srcRect.h = 800;
    // bounds    = srcRect;
    SDL_RenderCopy(image->renderer, image->texture, &srcRect, &srcRect);
}

Uint32 ConvertColor(const double red, const double green, const double blue)
{
    const unsigned char r = (unsigned char)(red);
    const unsigned char g = (unsigned char)(green);
    const unsigned char b = (unsigned char)(blue);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const Uint32 colour = (b << 24) + (g << 16) + (r << 8) + 255;
#else
    const Uint32 colour = (255 << 24) + (r << 16) + (g << 8) + b;
#endif

    return colour;
}

void Image_Free(Image_t *image)
{
    free(image->r);
    free(image->g);
    free(image->b);

    SDL_DestroyTexture(image->texture);

    image->r       = NULL;
    image->g       = NULL;
    image->b       = NULL;
    image->texture = NULL;

    image = NULL;
}