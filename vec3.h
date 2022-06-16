#ifndef __VEC3_H__
#define __VEC3_H__

#include <stdio.h>

typedef struct vector
{
    double x, y, z;
} vec3;

vec3 vec3_add(const vec3 v1, const vec3 v2)
{
    return (vec3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

vec3 vec3_sub(const vec3 v1, const vec3 v2)
{
    return (vec3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

vec3 vec3_mul(const vec3 v1, const vec3 v2)
{
    return (vec3){v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
}

vec3 vec3_mul_scal(const vec3 v1, const double val)
{
    return (vec3){v1.x * val, v1.y * val, v1.z * val};
}

vec3 vec3_div(const vec3 v1, const vec3 v2)
{
    return (vec3){v1.x / v2.x, v1.y / v2.y, v1.z / v2.z};
}

vec3 vec3_div_scal(const vec3 v1, const double val)
{
    return (vec3){v1.x / val, v1.y / val, v1.z / val};
}

double vec3_length(const vec3 v1)
{
    const vec3 tmp = vec3_mul(v1, v1);
    return tmp.x + tmp.y + tmp.z;
}

#define NORMALISE_VEC3(V) (V) = vec3_normalise((V))

vec3 vec3_normalise(const vec3 v1)
{
    double length = vec3_length(v1);
    return (vec3){v1.x / length, v1.y / length, v1.z / length};
}

vec3 vec3_cross(const vec3 v1, const vec3 v2)
{
    return (vec3){
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x,
    };
}

#endif // __VEC3_H__