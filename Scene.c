#include "Scene.h"

#include "SDL2/SDL.h"
#include "Image.h"

static struct Scene_s
{
    Image_t output_image;
} Scene;

void Scene_Init()
{
    SDL_Texture *texture = SDL_CreateTexture(app.renderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_STREAMING, app.width, app.height);

    // Initialise image
    Image_t image      = Image_Initialize(800, 800, app.renderer);
    Scene.output_image = image;
}

void Scene_Update()
{
    const unsigned int x_size = Scene.output_image.x_size;
    const unsigned int y_size = Scene.output_image.y_size;

    for (unsigned int x = 0; x < x_size; x++)
    {
        for (unsigned int y = 0; y < y_size; y++)
        {
            const double r = ((double)x / (double)x_size) * 255.0;
            const double g = ((double)y / (double)y_size) * 255.0;

            Image_SetPixel(&Scene.output_image, x, y, r, g, 0.0);
        }
    }
    Image_Display(&Scene.output_image);
}

void Scene_Exit()
{
    Image_Free(&Scene.output_image);
}
