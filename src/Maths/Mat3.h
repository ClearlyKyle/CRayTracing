#ifndef __MAT3_H__
#define __MAT3_H__

#include "vec3.h"

typedef struct mat3
{
    double elements[3][3];
} mat3;

inline vec3 mat3_mul_vec3(const mat3 mat, const vec3 vec)
{
    vec3 out = {0};

    out.x = mat.elements[0][0] * vec.x + mat.elements[1][0] * vec.y + mat.elements[2][0] * vec.z;
    out.y = mat.elements[0][1] * vec.x + mat.elements[1][1] * vec.y + mat.elements[2][1] * vec.z;
    out.z = mat.elements[0][2] * vec.x + mat.elements[1][2] * vec.y + mat.elements[2][2] * vec.z;

    return out;
}

#endif // __MAT3_H__