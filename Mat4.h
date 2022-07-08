#ifndef __TRANSFORMS_H__
#define __TRANSFORMS_H__

#include "vec3.h"

typedef struct Mat4
{
    float elements[4][4];
} Mat4;

inline Mat4 Mat3_Transpose(const Mat4 m)
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

inline Mat4 Mat4_Scale_Make(const Mat4 m, const vec3 v)
{
    Mat4 dest = {0};

    dest.elements[0][0] = v.x;
    dest.elements[1][1] = v.y;
    dest.elements[2][2] = v.z;

    return dest;
}

inline Mat4 Mat4_ZRot_Make(const Mat4 m, const float angle)
{
    Mat4 dest = {0};

    dest.elements[0][0] = cosf(angle);
    dest.elements[0][1] = -sin(angle);
    dest.elements[1][0] = sin(angle);
    dest.elements[1][1] = cos(angle);

    return dest;
}

inline Mat4 Mat4_YRot_Make(const Mat4 m, const float angle)
{
    Mat4 dest = {0};

    dest.elements[0][0] = cosf(angle);
    dest.elements[0][2] = sin(angle);
    dest.elements[2][0] = -sin(angle);
    dest.elements[2][2] = cos(angle);

    return dest;
}

inline Mat4 Mat4_XRot_Make(const Mat4 m, const float angle)
{
    Mat4 dest = {0};

    dest.elements[1][1] = cosf(angle);
    dest.elements[1][2] = -sin(angle);
    dest.elements[2][1] = sin(angle);
    dest.elements[2][2] = cos(angle);

    return dest;
}

inline Mat4 Mat4_Make_Rotation(const vec3 angles)
{
    Mat4 dest = {0};

    const float sx = sinf(angles.x);
    const float cx = cosf(angles.x);
    const float sy = sinf(angles.y);
    const float cy = cosf(angles.y);
    const float sz = sinf(angles.z);
    const float cz = cosf(angles.z);

    const float czsx = cz * sx;
    const float cxcz = cx * cz;
    const float sysz = sy * sz;

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

inline Mat4 Mat4_Mul(const Mat4 m1, const Mat4 m2)
{
    const float a00 = m1.elements[0][0];
    const float a01 = m1.elements[0][1];
    const float a02 = m1.elements[0][2];
    const float a03 = m1.elements[0][3];
    const float a10 = m1.elements[1][0];
    const float a11 = m1.elements[1][1];
    const float a12 = m1.elements[1][2];
    const float a13 = m1.elements[1][3];
    const float a20 = m1.elements[2][0];
    const float a21 = m1.elements[2][1];
    const float a22 = m1.elements[2][2];
    const float a23 = m1.elements[2][3];
    const float a30 = m1.elements[3][0];
    const float a31 = m1.elements[3][1];
    const float a32 = m1.elements[3][2];
    const float a33 = m1.elements[3][3];
    const float b00 = m2.elements[0][0];
    const float b01 = m2.elements[0][1];
    const float b02 = m2.elements[0][2];
    const float b03 = m2.elements[0][3];
    const float b10 = m2.elements[1][0];
    const float b11 = m2.elements[1][1];
    const float b12 = m2.elements[1][2];
    const float b13 = m2.elements[1][3];
    const float b20 = m2.elements[2][0];
    const float b21 = m2.elements[2][1];
    const float b22 = m2.elements[2][2];
    const float b23 = m2.elements[2][3];
    const float b30 = m2.elements[3][0];
    const float b31 = m2.elements[3][1];
    const float b32 = m2.elements[3][2];
    const float b33 = m2.elements[3][3];

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

#endif // __TRANSFORMS_H__