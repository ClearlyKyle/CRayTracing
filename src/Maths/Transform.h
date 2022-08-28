#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include <stdbool.h>

#include "Mat4.h"
#include "../Ray.h"

typedef struct Tranform
{
    Mat4 forward;
    Mat4 backwards;
} Transform;

vec3      Transform_Apply_Forward(const Transform trans, const vec3 in_vec);
vec3      Transform_Apply_Backwards(const Transform trans, const vec3 in_vec);
vec3      Transform_Apply_Normal(const Transform trans, const vec3 normal);
Ray       Transform_Apply_Ray(Transform trans, Ray ray, bool direction);
Transform Transform_Set(const vec3 translation, const vec3 rotation, const vec3 scale);

#endif // __TRANSFORM_H__