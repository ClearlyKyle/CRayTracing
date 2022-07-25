#ifndef __LIGHTS_H__
#define __LIGHTS_H__

// -------------------------------------------------
// LIGHTS
#include "Light.h"

enum LightTypes
{
    LIGHT_POINT,
    LIGHT_COUNT,
};

typedef union
{
    Light_t *pointLight;
} LightObjects;

typedef struct LightArray
{
    enum LightTypes type;
    LightObjects object;
} LightArray;

typedef struct Lights
{
    size_t count;
    LightArray *lights;
} Lights;

inline void Lights_Free(Lights *lights)
{
    free(lights->lights);
    lights->lights = NULL;
};

#endif // __LIGHTS_H__