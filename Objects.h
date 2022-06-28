#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "Light.h"
#include "Sphere.h"

enum Shapes
{
    SHAPE_SPHERE,
    SHAPE_COUNT,
};

enum Lights
{
    LIGHT_POINT,
    LIGHT_COUNT,
};

typedef union
{
    Sphere_t *sphere;
} ShapeObjects;

typedef union
{
    Light_t *pointLight;
} LightObjects;

typedef struct shape_array_data
{
    enum Shape_Types type;
    ShapeObjects     object;
} ShapeArray_t;

typedef struct light_array_data
{
    enum Lights  type;
    LightObjects object;
} LightArray_t;

#endif // __OBJECTS_H__