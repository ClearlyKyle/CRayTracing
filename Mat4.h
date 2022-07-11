#ifndef __TRANSFORMS_H__
#define __TRANSFORMS_H__

#include "vec3.h"
#include "vec4.h"

typedef struct Mat4
{
    double elements[4][4];
} Mat4;

inline Mat4 Mat4_Transpose(const Mat4 m)
{
    Mat4 dest = {0};

    dest.elements[0][0] = m.elements[0][0];
    dest.elements[1][0] = m.elements[0][1];
    dest.elements[0][1] = m.elements[1][0];
    dest.elements[1][1] = m.elements[1][1];
    dest.elements[0][2] = m.elements[2][0];
    dest.elements[1][2] = m.elements[2][1];
    dest.elements[0][3] = m.elements[3][0];
    dest.elements[1][3] = m.elements[3][1];
    dest.elements[2][0] = m.elements[0][2];
    dest.elements[3][0] = m.elements[0][3];
    dest.elements[2][1] = m.elements[1][2];
    dest.elements[3][1] = m.elements[1][3];
    dest.elements[2][2] = m.elements[2][2];
    dest.elements[3][2] = m.elements[2][3];
    dest.elements[2][3] = m.elements[3][2];
    dest.elements[3][3] = m.elements[3][3];

    return dest;
}

inline void Mat4_Scale(Mat4 *const m, const double s)
{
    m->elements[0][0] *= s;
    m->elements[0][1] *= s;
    m->elements[0][2] *= s;
    m->elements[0][3] *= s;
    m->elements[1][0] *= s;
    m->elements[1][1] *= s;
    m->elements[1][2] *= s;
    m->elements[1][3] *= s;
    m->elements[2][0] *= s;
    m->elements[2][1] *= s;
    m->elements[2][2] *= s;
    m->elements[2][3] *= s;
    m->elements[3][0] *= s;
    m->elements[3][1] *= s;
    m->elements[3][2] *= s;
    m->elements[3][3] *= s;
}

inline Mat4 Mat4_Scale_Make(const vec3 v)
{
    Mat4 dest = {0};

    dest.elements[0][0] = v.x;
    dest.elements[1][1] = v.y;
    dest.elements[2][2] = v.z;
    dest.elements[3][3] = 1.0;

    return dest;
}

inline Mat4 Mat4_ZRot_Make(const double angle)
{
    Mat4 dest = {0};

    dest.elements[0][0] = cos(angle);
    dest.elements[0][1] = -sin(angle);
    dest.elements[1][0] = sin(angle);
    dest.elements[1][1] = cos(angle);

    dest.elements[2][2] = 1.0;
    dest.elements[3][3] = 1.0;

    return dest;
}

inline Mat4 Mat4_YRot_Make(const double angle)
{
    Mat4 dest = {0};

    dest.elements[0][0] = cos(angle);
    dest.elements[0][2] = sin(angle);
    dest.elements[2][0] = -sin(angle);
    dest.elements[2][2] = cos(angle);

    dest.elements[1][1] = 1.0;
    dest.elements[3][3] = 1.0;

    return dest;
}

inline Mat4 Mat4_XRot_Make(const double angle)
{
    Mat4 dest = {0};

    dest.elements[1][1] = cos(angle);
    dest.elements[1][2] = -sin(angle);
    dest.elements[2][1] = sin(angle);
    dest.elements[2][2] = cos(angle);

    dest.elements[0][0] = 1.0;
    dest.elements[3][3] = 1.0;

    return dest;
}

inline Mat4 Mat4_Make_Rotation(const vec3 angles)
{
    Mat4 dest = {0};

    const double sx = sin(angles.x);
    const double cx = cos(angles.x);
    const double sy = sin(angles.y);
    const double cy = cos(angles.y);
    const double sz = sin(angles.z);
    const double cz = cos(angles.z);

    const double czsx = cz * sx;
    const double cxcz = cx * cz;
    const double sysz = sy * sz;

    dest.elements[0][0] = cy * cz;
    dest.elements[0][1] = czsx * sy + cx * sz;
    dest.elements[0][2] = -cxcz * sy + sx * sz;
    dest.elements[1][0] = -cy * sz;
    dest.elements[1][1] = cxcz - sx * sysz;
    dest.elements[1][2] = czsx + cx * sysz;
    dest.elements[2][0] = sy;
    dest.elements[2][1] = -cy * sx;
    dest.elements[2][2] = cx * cy;
    dest.elements[0][3] = 0.0f;
    dest.elements[1][3] = 0.0f;
    dest.elements[2][3] = 0.0f;
    dest.elements[3][0] = 0.0f;
    dest.elements[3][1] = 0.0f;
    dest.elements[3][2] = 0.0f;
    dest.elements[3][3] = 1.0f;

    return dest;
}

inline Mat4 Mat4_Mul_Mat4(const Mat4 m1, const Mat4 m2)
{
    const double a00 = m1.elements[0][0];
    const double a01 = m1.elements[0][1];
    const double a02 = m1.elements[0][2];
    const double a03 = m1.elements[0][3];
    const double a10 = m1.elements[1][0];
    const double a11 = m1.elements[1][1];
    const double a12 = m1.elements[1][2];
    const double a13 = m1.elements[1][3];
    const double a20 = m1.elements[2][0];
    const double a21 = m1.elements[2][1];
    const double a22 = m1.elements[2][2];
    const double a23 = m1.elements[2][3];
    const double a30 = m1.elements[3][0];
    const double a31 = m1.elements[3][1];
    const double a32 = m1.elements[3][2];
    const double a33 = m1.elements[3][3];
    const double b00 = m2.elements[0][0];
    const double b01 = m2.elements[0][1];
    const double b02 = m2.elements[0][2];
    const double b03 = m2.elements[0][3];
    const double b10 = m2.elements[1][0];
    const double b11 = m2.elements[1][1];
    const double b12 = m2.elements[1][2];
    const double b13 = m2.elements[1][3];
    const double b20 = m2.elements[2][0];
    const double b21 = m2.elements[2][1];
    const double b22 = m2.elements[2][2];
    const double b23 = m2.elements[2][3];
    const double b30 = m2.elements[3][0];
    const double b31 = m2.elements[3][1];
    const double b32 = m2.elements[3][2];
    const double b33 = m2.elements[3][3];

    Mat4 dest = {0};

    dest.elements[0][0] = a00 * b00 + a10 * b01 + a20 * b02 + a30 * b03;
    dest.elements[0][1] = a01 * b00 + a11 * b01 + a21 * b02 + a31 * b03;
    dest.elements[0][2] = a02 * b00 + a12 * b01 + a22 * b02 + a32 * b03;
    dest.elements[0][3] = a03 * b00 + a13 * b01 + a23 * b02 + a33 * b03;
    dest.elements[1][0] = a00 * b10 + a10 * b11 + a20 * b12 + a30 * b13;
    dest.elements[1][1] = a01 * b10 + a11 * b11 + a21 * b12 + a31 * b13;
    dest.elements[1][2] = a02 * b10 + a12 * b11 + a22 * b12 + a32 * b13;
    dest.elements[1][3] = a03 * b10 + a13 * b11 + a23 * b12 + a33 * b13;
    dest.elements[2][0] = a00 * b20 + a10 * b21 + a20 * b22 + a30 * b23;
    dest.elements[2][1] = a01 * b20 + a11 * b21 + a21 * b22 + a31 * b23;
    dest.elements[2][2] = a02 * b20 + a12 * b21 + a22 * b22 + a32 * b23;
    dest.elements[2][3] = a03 * b20 + a13 * b21 + a23 * b22 + a33 * b23;
    dest.elements[3][0] = a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33;
    dest.elements[3][1] = a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33;
    dest.elements[3][2] = a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33;
    dest.elements[3][3] = a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33;

    return dest;
}

inline Mat4 Mat4_Make_Transform(const vec3 v)
{
    Mat4 dest = {0};

    dest.elements[0][3] = v.x;
    dest.elements[1][3] = v.y;
    dest.elements[2][3] = v.z;

    dest.elements[0][0] = 1.0;
    dest.elements[1][1] = 1.0;
    dest.elements[2][2] = 1.0;
    dest.elements[3][3] = 1.0;

    return dest;
}

inline vec4 Mat4_mul_vec4(const Mat4 m, const vec4 v)
{
    vec4 dest = {0};
    dest.x    = m.elements[0][0] * v.x + m.elements[1][0] * v.y + m.elements[2][0] * v.z + m.elements[3][0] * v.w;
    dest.y    = m.elements[0][1] * v.x + m.elements[1][1] * v.y + m.elements[2][1] * v.z + m.elements[3][1] * v.w;
    dest.z    = m.elements[0][2] * v.x + m.elements[1][2] * v.y + m.elements[2][2] * v.z + m.elements[3][2] * v.w;
    dest.w    = m.elements[0][3] * v.x + m.elements[1][3] * v.y + m.elements[2][3] * v.z + m.elements[3][3] * v.w;

    return dest;
}

inline Mat4 Mat4_Inverse(const Mat4 mat)
{
    Mat4 dest = {0};

    double t[6];
    double det;
    double a = mat.elements[0][0], b = mat.elements[0][1], c = mat.elements[0][2], d = mat.elements[0][3],
           e = mat.elements[1][0], f = mat.elements[1][1], g = mat.elements[1][2], h = mat.elements[1][3],
           i = mat.elements[2][0], j = mat.elements[2][1], k = mat.elements[2][2], l = mat.elements[2][3],
           m = mat.elements[3][0], n = mat.elements[3][1], o = mat.elements[3][2], p = mat.elements[3][3];

    t[0] = k * p - o * l;
    t[1] = j * p - n * l;
    t[2] = j * o - n * k;
    t[3] = i * p - m * l;
    t[4] = i * o - m * k;
    t[5] = i * n - m * j;

    dest.elements[0][0] = f * t[0] - g * t[1] + h * t[2];
    dest.elements[1][0] = -(e * t[0] - g * t[3] + h * t[4]);
    dest.elements[2][0] = e * t[1] - f * t[3] + h * t[5];
    dest.elements[3][0] = -(e * t[2] - f * t[4] + g * t[5]);

    dest.elements[0][1] = -(b * t[0] - c * t[1] + d * t[2]);
    dest.elements[1][1] = a * t[0] - c * t[3] + d * t[4];
    dest.elements[2][1] = -(a * t[1] - b * t[3] + d * t[5]);
    dest.elements[3][1] = a * t[2] - b * t[4] + c * t[5];

    t[0] = g * p - o * h;
    t[1] = f * p - n * h;
    t[2] = f * o - n * g;
    t[3] = e * p - m * h;
    t[4] = e * o - m * g;
    t[5] = e * n - m * f;

    dest.elements[0][2] = b * t[0] - c * t[1] + d * t[2];
    dest.elements[1][2] = -(a * t[0] - c * t[3] + d * t[4]);
    dest.elements[2][2] = a * t[1] - b * t[3] + d * t[5];
    dest.elements[3][2] = -(a * t[2] - b * t[4] + c * t[5]);

    t[0] = g * l - k * h;
    t[1] = f * l - j * h;
    t[2] = f * k - j * g;
    t[3] = e * l - i * h;
    t[4] = e * k - i * g;
    t[5] = e * j - i * f;

    dest.elements[0][3] = -(b * t[0] - c * t[1] + d * t[2]);
    dest.elements[1][3] = a * t[0] - c * t[3] + d * t[4];
    dest.elements[2][3] = -(a * t[1] - b * t[3] + d * t[5]);
    dest.elements[3][3] = a * t[2] - b * t[4] + c * t[5];

    det = 1.0 / (a * dest.elements[0][0] + b * dest.elements[1][0] + c * dest.elements[2][0] + d * dest.elements[3][0]);

    Mat4_Scale(&dest, det);

    return dest;
}

#endif // __TRANSFORMS_H__