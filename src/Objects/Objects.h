#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include <stdlib.h>
#include "../Maths/MathsHeader.h"
#include "../Textures/Textures.h"

typedef struct Material
{
    double reflectivity;
    double shininess;

    vec3 base_colour;

    Texture texture;
    bool    has_texture;
} Material;

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
    enum Shape_Types type;
    bool             visible;
    vec3             base_colour;
    vec2             uv_coordinates;
    Transform        transform;
    Material        *mat;
} Shape;

typedef bool (*shape_test_intersection)(Shape *const shape,
                                        const Ray    ray,
                                        vec3 *const  int_point,
                                        vec3 *const  local_normal,
                                        vec3 *const  local_colour);

// -------------------------------------------------
// Intersection Testing

#define CLOSE_ENOUGH(D1, D2) fabs((D1) - (D2)) < 1e-21f

bool Plane_Test_Intersection(Shape *const shape,
                             const Ray    ray,
                             vec3 *const  int_point,
                             vec3 *const  local_normal,
                             vec3 *const  local_colour);

bool Sphere_Test_Intersection(Shape *const shape,
                              const Ray    ray,
                              vec3 *const  int_point,
                              vec3 *const  local_normal,
                              vec3 *const  local_colour);

bool Cylinder_Test_Intersection(Shape *const shape,
                                const Ray    ray,
                                vec3 *const  int_point,
                                vec3 *const  local_normal,
                                vec3 *const  local_colour);

bool Cone_Test_Intersection(Shape *const shape,
                            const Ray    ray,
                            vec3 *const  int_point,
                            vec3 *const  local_normal,
                            vec3 *const  local_colour);

const static shape_test_intersection _intersection_functions[] = {
    Sphere_Test_Intersection,
    Plane_Test_Intersection,
    Cylinder_Test_Intersection,
    Cone_Test_Intersection,
};

// -------------------------------------------------
// Objects

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

inline bool Object_Test_Intersection(Shape *const shape,
                                     const Ray    ray,
                                     vec3 *const  int_point,
                                     vec3 *const  local_normal,
                                     vec3 *const  local_colour)
{
    return _intersection_functions[shape->type]((Shape *const)shape, ray, int_point, local_normal, local_colour);
}

inline void Objects_Free(Objects *objects)
{
    free(objects->shapes);
    objects->shapes = NULL;
};

#endif // __OBJECTS_H__