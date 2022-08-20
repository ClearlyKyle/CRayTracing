#include "Textures.h"

inline Texture Texture_Init_Flat(const vec4 colour)
{
    Texture t      = {0};
    t.type         = TEXTURE_FLAT;
    t.flat         = malloc(sizeof(struct Flat));
    t.flat->colour = colour;

    return t;
}

vec4 Texture_Flat_Get_Colour(const struct Flat flat)
{
    return flat.colour;
}