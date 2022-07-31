#ifndef __SPHERE_H__
#define __SPHERE_H__

#include <stdbool.h>
#include "vec3.h"
#include "Ray.h"
#include "Transform.h"
#include "Material.h"




bool Sphere_Test_Intersection(const Sphere sphere,
                              const Ray_t *ray,
                              vec3        *int_point,
                              vec3        *local_normal,
                              vec3        *local_colour);

#endif // __SPHERE_H__