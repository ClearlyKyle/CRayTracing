#ifndef __SPHERE_H__
#define __SPHERE_H__

#include <stdbool.h>
#include "vec3.h"
#include "Ray.h"

struct Sphere
{
    vec3 colour;
};

typedef struct Sphere Sphere_t;

bool Sphere_Test_Intersection(const Ray_t *ray,
                              vec3        *int_point,
                              vec3        *local_normal,
                              vec3         local_colour);

#endif // __SPHERE_H__