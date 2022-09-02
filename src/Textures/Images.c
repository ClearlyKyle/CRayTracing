#include "Textures.h"

#include "SDL2/SDL.h"

// Load Image to be used as a texture
Texture Texture_Image_Init(const char  *file,
                           const vec2   translation,
                           const double rotation,
                           const vec2   scale)
{
    Texture t = {0};
    t.type    = TEXTURE_IMAGE;
    // t.image->file_name = file;

    SDL_Surface *surface = SDL_LoadBMP(file);

    if (!surface)
    {
        fprintf(stderr, "Failed to load image: %s\nSDL_GetError - %s\n", file, SDL_GetError());
        return t;
    }

    fprintf(stdout, "Image laoded : %s\n", file);
    fprintf(stdout, "%dx%d, %2dbpp, %2dpitch\n", surface->w, surface->h, surface->format->BitsPerPixel, surface->pitch);

    t.image                   = malloc(sizeof(struct Image));
    t.image->image_surface    = surface;
    t.image->transform_matrix = _Texture_Create_Transform_Matrix(translation, rotation, scale);

    return t;
}

vec4 Texture_Image_Get_Colour(const struct Image *image, const vec2 uv_coords)
{
    if (!image->image_surface)
    {
        fprintf(stderr, "Using 'Texture_Image_Get_Colour' without a valid image!\n");
        return (vec4){1.0, 0.0, 1.0, 1.0};
    }

    // Apply the local transform to the (u,v) coordinates.
    const vec2 new_location = Texture_Apply_Transform(image->transform_matrix, uv_coords);

    const double u = new_location.x;
    const double v = new_location.y;

    const int width  = image->image_surface->w;
    const int height = image->image_surface->h;

    // Convert (u,v) to image dimensions (x,y).
    int x = (int)(round(((u + 1.0) / 2.0) * (double)(width)));
    int y = height - ((int)(round(((v + 1.0) / 2.0) * (double)(height))));

    x = ((x % width) + width) % width;
    y = ((y % height) + height) % height;

    // Verify that we are within the image.
    // Probably not necessary, but seems like a good idea just in case.
    if ((x >= 0) && (x < width) && (y >= 0) && (y < height))
    {
        const int bpp = image->image_surface->format->BytesPerPixel;

        Uint32 *const pixel = (Uint32 *)((Uint8 *)image->image_surface->pixels + y * image->image_surface->pitch + x * bpp);

        // Convert to RGB.
        uint8_t r, g, b, a;
        SDL_GetRGBA(*pixel, image->image_surface->format, &r, &g, &b, &a);

        // Set the return vector accordingly.
        return (vec4){(double)(r) / 255.0,
                      (double)(g) / 255.0,
                      (double)(b) / 255.0,
                      (double)(a) / 255.0};
    }

    return (vec4){0.0, 0.0, 0.0, 1.0};
}