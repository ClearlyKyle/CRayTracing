#include "Textures.h"

vec3 Texture_Blend_Colours(const vec3 colour_list[])
{
    return (vec3){0.0, 0.0, 0.0};
}

vec3 Texture_Apply_Transform(const vec3 intput)
{
    vec3 result = mat3_mul_vec3(transformation, input);

    return (vec2){result.x, result.y};
}