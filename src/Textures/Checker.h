#ifndef __CHECKER_H__
#define __CHECKER_H__

#include "Textures.h"

inline Texture Texture_Init_Flat(const mat3 transfor_matrix, const vec4 colour1, const vec4 colour2)
{
    Texture t                   = {0};
    t.type                      = TEXTURE_FLAT;
    t.checker                   = malloc(sizeof(struct Checker));
    t.checker->colour1          = colour1;
    t.checker->colour1          = colour2;
    t.checker->transform_matrix = transfor_matrix;

    return t;
}

vec4 Texture_Checker_Get_Colour(const struct Checker *checker, const vec2 uv_coords)
{
    // Apply the local transform to the (u,v) coordinates.
    vec2 new_location = Texture_Apply_Transform(checker->transform_matrix, uv_coords);

    const double newU = new_location.x;
    const double newV = new_location.y;

    const int check = (const int)(floor(newU)) + (int)(floor(newV));

    if ((check % 2) == 0)
    {
        return checker->colour1;
    }
    else
    {
        return checker->colour2;
    }
}

#endif // __CHECKER_H__