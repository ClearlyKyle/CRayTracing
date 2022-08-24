#include "Textures.h"

Texture Texture_Checker_Init(const vec2   translation,
                             const double rotation,
                             const vec2   scale,
                             const vec4   colour1,
                             const vec4   colour2)
{
    Texture t                   = {0};
    t.type                      = TEXTURE_CHECKER;
    t.checker                   = malloc(sizeof(struct Checker));
    t.checker->colour1          = colour1;
    t.checker->colour1          = colour2;
    t.checker->transform_matrix = _Texture_Create_Transform_Matrix(translation, rotation, scale);

    return t;
}

vec4 Texture_Checker_Get_Colour(const struct Checker *checker, const vec2 uv_coords)
{
    // Apply the local transform to the (u,v) coordinates.
    const vec2 new_location = Texture_Apply_Transform(checker->transform_matrix, uv_coords);

    const double newU = new_location.x;
    const double newV = new_location.y;

    const int check = (const int)(floor(newU)) + (int)(floor(newV));

    return ((check % 2) == 0) ? checker->colour1 : checker->colour2;
}