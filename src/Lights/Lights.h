#ifndef __LIGHTS_H__
#define __LIGHTS_H__

#include <stdlib.h>
#include <stdbool.h>

#include "../Objects/Objects.h"

// -------------------------------------------------
// LIGHTS

typedef struct
{
    vec3   location;
    vec3   colour;
    double intensity;
} PointLight;

enum LightTypes
{
    LIGHT_POINT,
    LIGHT_COUNT,
};

typedef union
{
    PointLight pointLight;
} LightObjects;

typedef struct LightArray
{
    enum LightTypes type;
    LightObjects    object;
} LightArray;

typedef struct Lights
{
    size_t      count;
    LightArray *lights;
} Lights;

inline void Lights_Free(Lights *lights)
{
    free(lights->lights);
    lights->lights = NULL;
};

bool Light_Compute_Illumination(const PointLight light,
                                const Objects    objects,
                                const size_t     current_object_index,
                                const vec3       intPoint,
                                const vec3       localNormal,
                                vec3 *const      colour,
                                double *const    intensity);

#endif // __LIGHTS_H__