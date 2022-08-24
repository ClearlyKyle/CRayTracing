#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include <stdio.h>
#include <stdlib.h>

#include "../Maths/MathsHeader.h"

struct Flat
{
    vec4 colour;
};

struct Checker
{
    mat3 transform_matrix;
    vec4 colour1;
    vec4 colour2;
};

// ------------------------------------------------------------------------------------------------

enum Texture_types
{
    TEXTURE_CHECKER,
    TEXTURE_FLAT,
    TEXTURE_COUNT
};

typedef struct Texture
{
    enum Texture_types type;
    union
    {
        struct Flat    *flat;
        struct Checker *checker;
    };
} Texture;

// ------------------------------------------------------------------------------------------------

static inline mat3 _Texture_Create_Transform_Matrix(const vec2 translation, const double rotation, const vec2 scale)
{
    const double c = cos(rotation);
    const double s = sin(rotation);

    const mat3 rotation_matrix = {c, s, 0.0,
                                  -s, c, 0.0,
                                  0.0, 0.0, 1.0};

    const mat3 scale_matrix = {scale.x, 0.0, 0.0,
                               0.0, scale.y, 0.0,
                               0.0, 0.0, 1.0};

    const mat3 translation_matrix = {1.0, 0.0, 0.0,
                                     0.0, 1.0, 0.0,
                                     translation.x, translation.y, 1.0};

    return mat3_mul_mat3(mat3_mul_mat3(translation_matrix, rotation_matrix), scale_matrix);
}

inline vec3 Texture_Blend_Colours(const vec3 colour_list[])
{
    return (vec3){0.0, 0.0, 0.0};
}

inline vec2 Texture_Apply_Transform(const mat3 transform_matrix, const vec2 input)
{
    const vec3 result = mat3_mul_vec3(transform_matrix, (vec3){input.x, input.y, 0.0});

    return (vec2){result.x, result.y};
}

inline void Texture_Free(Texture *const texture)
{
    switch (texture->type)
    {
    case TEXTURE_CHECKER:
        free(texture->checker);
        texture->checker = NULL;
        return;
    case TEXTURE_FLAT:
        free(texture->flat);
        texture->flat = NULL;
        break;

    default:
        break;
    }
}

// ------------------------------------------------------------------------------------------------
// Texture Get Colour

// CHECKER
vec4    Texture_Checker_Get_Colour(const struct Checker *checker, const vec2 uv_coords);
Texture Texture_Checker_Init(const vec2 translation, const double rotation, const vec2 scale, const vec4 colour1, const vec4 colour2);

inline vec4 Texture_Get_Colour(const Texture texture, const vec2 uv_coords)
{
    switch (texture.type)
    {
    case TEXTURE_CHECKER:
        return Texture_Checker_Get_Colour((const struct Checker *)(texture.checker), uv_coords);
    case TEXTURE_FLAT:
        return texture.flat->colour;

    default:
        fprintf(stderr, "Not supported Texture Type: %d\n", texture.type);
        break;
    }
    return (vec4){0.0, 0.0, 0.0, 1.0};
}
#endif // __TEXTURES_H__