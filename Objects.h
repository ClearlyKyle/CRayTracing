#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "Light.h"
#include "Sphere.h"
#include "Plane.h"

// -------------------------------------------------
// SHAPES

enum Shapes
{
    SHAPE_SPHERE,
    SHAPE_PLANE,
    SHAPE_COUNT,
};

typedef union ShapeObjects
{
    Sphere sphere;
    Plane  plane;
} ShapeObjects;

typedef struct ShapesArray
{
    enum Shape_Types type;
    ShapeObjects     object;
} ShapeArray;

// -------------------------------------------------
// LIGHTS

enum Lights
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
    enum Lights  type;
    LightObjects object;
} LightArray;

#endif // __OBJECTS_H__