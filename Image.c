#include "Image.h"

struct Image_s
{
    double *r_channel;
    double *g_channel;
    double *b_channel;

    unsigned int x_size;
    unsigned int y_size;

    SDL_Renderer *renderer;
    SDL_Texture  *texture;
};

static void Image_Init_Texture(const Image_t *image)
{
    // Initialise the texture.
    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask               = 0x000000ff;
    gmask               = 0x0000ff00;
    bmask               = 0x00ff0000;
    amask               = 0xff000000;
#endif

    // Delete any previously created texture before we create a new one.
    if (image.texture != NULL)
    {
        SDL_DestroyTexture(m_pTexture);
    }

    // TODO : Check this..
    SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, m_xSize, m_ySize, 32, rmask, gmask, bmask, amask);
    image.texture            = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
    SDL_FreeSurface(tempSurface);
}

Image_t Image_Initialize(const int x_size, const int y_size, SDL_Renderer *renderer)
{
    Image_t image;

    image.red    = (double *)malloc(sizeof(double) * y_size);
    image.greeen = (double *)malloc(sizeof(double) * y_size);
    image.blue   = (double *)malloc(sizeof(double) * y_size);

    image.x_size = x_size;
    image.y_size = y_size;

    image.renderer = renderer;

    Image_Init_Texture(&image);

    return image;
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
