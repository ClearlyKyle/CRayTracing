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

// void (*material_function_pointer)(int) = &fun;
// void (*shape_test_intersection)(int) = &fun;

// TODO : Switch shapes to this
// typedef struct Shape
//{
//     vec3      base_colour;
//     Transform transform;
//     Material  material;
//     bool      has_material;
// } Shape

// -------------------------------------------------
// SHAPES

typedef struct
{
    vec3      colour;
    Transform transform;
} Plane;

typedef struct
{
    vec3      colour;
    Transform transform;
} Sphere;

typedef struct
{
    vec3      colour;
    Transform transform;
} Cone;

// -------------------------------------------------
enum Shapes
{
    SHAPE_SPHERE,
    SHAPE_PLANE,
    SHAPE_CONE,
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
    Material        *mat;
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

bool Plane_Test_Intersection(const Plane  plane,
                             const Ray_t *ray,
                             vec3        *int_point,
                             vec3        *local_normal,
                             vec3        *local_colour);

bool Sphere_Test_Intersection(const Sphere sphere,
                              const Ray_t *ray,
                              vec3        *int_point,
                              vec3        *local_normal,
                              vec3        *local_colour);

#endif // __OBJECTS_H__