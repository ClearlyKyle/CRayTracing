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

inline mat3 mat3_mul_mat3(const mat3 mat1, const mat3 mat2)
{
    mat3 dest = {0};

    double a00 = mat1.elements[0][0], a01 = mat1.elements[0][1], a02 = mat1.elements[0][2],
           a10 = mat1.elements[1][0], a11 = mat1.elements[1][1], a12 = mat1.elements[1][2],
           a20 = mat1.elements[2][0], a21 = mat1.elements[2][1], a22 = mat1.elements[2][2],

           b00 = mat2.elements[0][0], b01 = mat2.elements[0][1], b02 = mat2.elements[0][2],
           b10 = mat2.elements[1][0], b11 = mat2.elements[1][1], b12 = mat2.elements[1][2],
           b20 = mat2.elements[2][0], b21 = mat2.elements[2][1], b22 = mat2.elements[2][2];

    dest.elements[0][0] = a00 * b00 + a10 * b01 + a20 * b02;
    dest.elements[0][1] = a01 * b00 + a11 * b01 + a21 * b02;
    dest.elements[0][2] = a02 * b00 + a12 * b01 + a22 * b02;
    dest.elements[1][0] = a00 * b10 + a10 * b11 + a20 * b12;
    dest.elements[1][1] = a01 * b10 + a11 * b11 + a21 * b12;
    dest.elements[1][2] = a02 * b10 + a12 * b11 + a22 * b12;
    dest.elements[2][0] = a00 * b20 + a10 * b21 + a20 * b22;
    dest.elements[2][1] = a01 * b20 + a11 * b21 + a21 * b22;
    dest.elements[2][2] = a02 * b20 + a12 * b21 + a22 * b22;

    return dest;
}

inline mat3 mat3_scale(const mat3 mat, const double s)
{
    mat3 dest = mat;

    dest.elements[0][0] *= s;
    dest.elements[0][1] *= s;
    dest.elements[0][2] *= s;
    dest.elements[1][0] *= s;
    dest.elements[1][1] *= s;
    dest.elements[1][2] *= s;
    dest.elements[2][0] *= s;
    dest.elements[2][1] *= s;
    dest.elements[2][2] *= s;

    return dest;
}

inline mat3 mat3_inverse(const mat3 mat)
{
    mat3 dest = {0};

    double       det;
    const double a = mat.elements[0][0], b = mat.elements[0][1], c = mat.elements[0][2],
                 d = mat.elements[1][0], e = mat.elements[1][1], f = mat.elements[1][2],
                 g = mat.elements[2][0], h = mat.elements[2][1], i = mat.elements[2][2];

    dest.elements[0][0] = e * i - f * h;
    dest.elements[0][1] = -(b * i - h * c);
    dest.elements[0][2] = b * f - e * c;
    dest.elements[1][0] = -(d * i - g * f);
    dest.elements[1][1] = a * i - c * g;
    dest.elements[1][2] = -(a * f - d * c);
    dest.elements[2][0] = d * h - g * e;
    dest.elements[2][1] = -(a * h - g * b);
    dest.elements[2][2] = a * e - b * d;

    det = 1.0 / (a * dest.elements[0][0] + b * dest.elements[1][0] + c * dest.elements[2][0]);

    return mat3_scale(dest, det);
}

inline void mat3_transpose(mat3 *mat)
{
    mat3 tmp = {0};

    tmp.elements[0][1] = mat->elements[1][0];
    tmp.elements[0][2] = mat->elements[2][0];
    tmp.elements[1][0] = mat->elements[0][1];
    tmp.elements[1][2] = mat->elements[2][1];
    tmp.elements[2][0] = mat->elements[0][2];
    tmp.elements[2][1] = mat->elements[1][2];

    mat->elements[0][1] = tmp.elements[0][1];
    mat->elements[0][2] = tmp.elements[0][2];
    mat->elements[1][0] = tmp.elements[1][0];
    mat->elements[1][2] = tmp.elements[1][2];
    mat->elements[2][0] = tmp.elements[2][0];
    mat->elements[2][1] = tmp.elements[2][1];
}

// source matrix will not be transposed unless dest is mat
inline mat3 mat3_transpose_to(mat3 mat)
{
    mat3 dest = {0};

    dest.elements[0][0] = mat.elements[0][0];
    dest.elements[0][1] = mat.elements[1][0];
    dest.elements[0][2] = mat.elements[2][0];
    dest.elements[1][0] = mat.elements[0][1];
    dest.elements[1][1] = mat.elements[1][1];
    dest.elements[1][2] = mat.elements[2][1];
    dest.elements[2][0] = mat.elements[0][2];
    dest.elements[2][1] = mat.elements[1][2];
    dest.elements[2][2] = mat.elements[2][2];

    return dest;
}

#endif // __MAT3_H__