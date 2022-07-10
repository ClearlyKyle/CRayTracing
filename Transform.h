#ifndef __TRANFORM_H__
#define __TRANFORM_H__

#include <stdbool.h>

#include "Mat4.h"
#include "Ray.h"

typedef struct Tranform
{
    Mat4 forward;
    Mat4 backwards;
} Transform;

vec3      Transform_Apply_Forward(const Transform trans, const vec3 in_vec);
vec3      Transform_Apply_Backwards(const Transform trans, const vec3 in_vec);
Ray_t     Transform_Apply_Ray(Transform trans, Ray_t ray, bool direction);
Transform Transform_Set(const vec3 translation, const vec3 rotation, const vec3 scale);

#endif // __TRANFORM_H__