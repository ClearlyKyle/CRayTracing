#ifndef __VEC3_H__
#define __VEC3_H__

#include <stdio.h>
#include <math.h>

//-----------------------------------------------------------
typedef struct vector
{
    double x, y, z;
} vec3;

// MACROS
//-----------------------------------------------------------
#define VEC3_INIT_ZERO \
    (vec3) { 0.0f, 0.0f, 0.0f }
#define VEC3_INIT_VALUE(VAL) \
    (vec3) { (VAL), (VAL), (VAL) }

// FUNCTIONS
//-----------------------------------------------------------

inline vec3 vec3_add(const vec3 v1, const vec3 v2)
{
    return (vec3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

inline vec3 vec3_sub(const vec3 v1, const vec3 v2)
{
    return (vec3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

inline vec3 vec3_mul(const vec3 v1, const vec3 v2)
{
    return (vec3){v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
}

inline vec3 vec3_mul_scal(const vec3 v1, const double val)
{
    return (vec3){v1.x * val, v1.y * val, v1.z * val};
}

inline vec3 vec3_div(const vec3 v1, const vec3 v2)
{
    return (vec3){v1.x / v2.x, v1.y / v2.y, v1.z / v2.z};
}

inline vec3 vec3_div_scal(const vec3 v1, const double val)
{
    return (vec3){v1.x / val, v1.y / val, v1.z / val};
}

inline double vec3_length(const vec3 v1)
{
    const vec3 tmp = vec3_mul(v1, v1);
    return sqrt(tmp.x + tmp.y + tmp.z);
}

#define NORMALISE_VEC3(V) (V) = vec3_normalise((V))

inline vec3 vec3_normalise(const vec3 v1)
{
    double length = vec3_length(v1);
    return (vec3){v1.x / length, v1.y / length, v1.z / length};
}

inline vec3 vec3_cross(const vec3 v1, const vec3 v2)
{
    return (vec3){
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x,
    };
}

inline double vec3_dot(const vec3 a, const vec3 b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

inline vec3 vec3_reflection(const vec3 ray, const vec3 normal)
{
    // ray - (2 * dot(ray, normal) * normal)
    const vec3 rhs = vec3_mul_scal(normal, vec3_dot(ray, normal));

    return vec3_sub(ray, vec3_mul_scal(rhs, 2.0));
}

#endif // __VEC3_H__