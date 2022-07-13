#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <stdbool.h>
#include "vec3.h"

#include "Objects.h"

struct Light_s
{
    vec3   location;
    vec3   colour;
    double intensity;
};

typedef struct Light_s Light_t;

bool Light_Compute_Illumination(const Light_t   *light,
                                const size_t     current_object,
                                const ShapeArray shapes[],
                                const size_t     num_shapes,
                                const vec3       intPoint,
                                const vec3       localNormal,
                                vec3            *colour,
                                double          *intensity);

#endif // __LIGHT_H__