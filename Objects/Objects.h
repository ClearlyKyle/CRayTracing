#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include <stdlib.h>
#include "../Transform.h"

typedef struct Material
{
    double reflectivity;
    double shininess;

    vec3 base_colour;
} Material;

// -------------------------------------------------
// SHAPES

typedef struct
{
    vec3      colour;
    Transform transform;

    Material *material;
} Plane;

typedef struct
{
    vec3      colour;
    Transform transform;

    Material *mat;
} Sphere;

// -------------------------------------------------
enum Shapes
{
    SHAPE_SPHERE,
    SHAPE_PLANE,
    SHAPE_COUNT,
};

typedef union
{
    Sphere sphere;
    Plane  plane;
} ShapeObjects;

typedef struct ShapesArray
{
    enum Shape_Types type;
    ShapeObjects     object;
} ShapeArray;

typedef struct Objects
{
    size_t      count;
    ShapeArray *shapes;
} Objects;

inline void Objects_Free(Objects *objects)
{
    free(objects->shapes);
    objects->shapes = NULL;
};

#endif // __OBJECTS_H__