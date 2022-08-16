#ifndef __CHECKER_H__
#define __CHECKER_H__

#include "../vec4.h"
#include "../vec2.h"

#include "Textures.h"

typedef struct Checker
{
    vec4 colour1;
    vec4 colour2;
} Checker;

// Function to return the color.
vec4 Texture_Checker_Get_Colour(const Checker checker, const vec2 uv_coords)
{
    // Apply the local transform to the (u,v) coordinates.
    vec2 new_location = Texture_Apply_Transform(transform, uv_coords);

    const double newU = new_location.x;
    const double newV = new_location.y;

    const int check = (const int)(floor(newU)) + (int)(floor(newV));

    if ((check % 2) == 0)
    {
        return checker.colour1;
    }
    else
    {
        return checker.colour2;
    }
}

#endif // __CHECKER_H__