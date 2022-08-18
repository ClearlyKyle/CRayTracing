#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include <stdlib.h>
#include "../Maths/MathsHeader.h"

typedef struct Material
{
    double reflectivity;
    double shininess;

    vec3 base_colour;
} Material;

typedef bool (*shape_test_intersection)(const Transform transform,
                                        const vec3      base_colour,
                                        const Ray       ray,
                                        vec3 *const     int_point,
                                        vec3 *const     local_normal,
                                        vec3 *const     local_colour);

// -------------------------------------------------
// SHAPES

enum Shapes
{
    SHAPE_SPHERE,
    SHAPE_PLANE,
    SHAPE_CYLINDER,
    SHAPE_CONE,
    SHAPE_COUNT,
};

typedef struct Shape
{
    enum Shape_Types        type;
    vec3                    base_colour;
    Transform               transform;
    Material               *mat;
    shape_test_intersection Test_Intersection;
} Shape;

// -------------------------------------------------

typedef struct Objects
{
    size_t count;
    Shape *shapes;
} Objects;

inline Objects Objects_Init(const size_t number_of_objects)
{
    Objects objs = {0};

    objs.count  = number_of_objects;
    objs.shapes = (Shape *)malloc(sizeof(Shape) * number_of_objects);
    if (!objs.shapes)
        fprintf(stderr, "Error! (Shape *)malloc(sizeof(Shape) * number_of_objects);\n");

    return objs;
}

inline void Objects_Free(Objects *objects)
{
    free(objects->shapes);
    objects->shapes = NULL;
};

#define CLOSE_ENOUGH(D1, D2) fabs((D1) - (D2)) < 1e-21f

bool Plane_Test_Intersection(const Transform transform,
                             const vec3      base_colour,
                             const Ray       ray,
                             vec3 *const     int_point,
                             vec3 *const     local_normal,
                             vec3 *const     local_colour);

bool Sphere_Test_Intersection(const Transform transform,
                              const vec3      base_colour,
                              const Ray       ray,
                              vec3 *const     int_point,
                              vec3 *const     local_normal,
                              vec3 *const     local_colour);

bool Cylinder_Test_Intersection(const Transform transform,
                                const vec3      base_colour,
                                const Ray       ray,
                                vec3 *const     int_point,
                                vec3 *const     local_normal,
                                vec3 *const     local_colour);

bool Cone_Test_Intersection(const Transform transform,
                            const vec3      base_colour,
                            const Ray       ray,
                            vec3 *const     int_point,
                            vec3 *const     local_normal,
                            vec3 *const     local_colour);

#endif // __OBJECTS_H__