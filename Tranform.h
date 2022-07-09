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

inline vec3 Transform_Apply_Forward(const Transform trans, const vec3 in_vec)
{
    // convert vec3 to vec4
    const vec4 tmp = {in_vec.x, in_vec.y, in_vec.z, 1.0f};

    // Apply forward transform
    const vec4 res = Mat4_mul_vec4(trans.forward, tmp);

    return (vec3){res.x, res.y, res.z};
}

inline vec3 Transform_Apply_Backwards(const Transform trans, const vec3 in_vec)
{
    // convert vec3 to vec4
    const vec4 tmp = {in_vec.x, in_vec.y, in_vec.z, 1.0f};

    // Apply forward transform
    const vec4 res = Mat4_mul_vec4(trans.backwards, tmp);

    return (vec3){res.x, res.y, res.z};
}

// direction = true = Forward
inline Ray_t Transform_Apply_Ray(Transform trans, Ray_t ray, bool direction)
{
    Ray_t ret;

    if (direction) // ENUM?
    {
        // Apply forward
        ret.point1 = Transform_Apply_Forward(trans, ray.point1);
        ret.point2 = Transform_Apply_Forward(trans, ray.point2);
        ret.lab    = vec3_sub(ret.point2, ret.point1);
    }
    else
    {
        // Apply backwards
        ret.point1 = Transform_Apply_Backwards(trans, ray.point1);
        ret.point2 = Transform_Apply_Backwards(trans, ray.point2);
        ret.lab    = vec3_sub(ret.point2, ret.point1);
    }

    return ret;
}

#endif // __TRANFORM_H__