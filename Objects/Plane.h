#ifndef __PLANE_H__
#define __PLANE_H__

#include <stdbool.h>

#include "../Transform.h"
#include "../vec3.h"
#include "../Ray.h"
#include "../Material.h"

bool Plane_Test_Intersection(const Plane plane, const Ray_t *ray, vec3 *int_point, vec3 *local_normal, vec3 *local_colour);

#endif // __PLANE_H__