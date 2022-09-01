#include "Transform.h"

vec3 Transform_Apply_Forward(const Transform trans, const vec3 in_vec)
{
    // convert vec3 to vec4
    const vec4 tmp = {in_vec.x, in_vec.y, in_vec.z, 1.0};

    // Apply forward transform
    const vec4 res = Mat4_mul_vec4(trans.forward, tmp);

    return (vec3){res.x, res.y, res.z};
}

vec3 Transform_Apply_Backwards(const Transform trans, const vec3 in_vec)
{
    // convert vec3 to vec4
    const vec4 tmp = {in_vec.x, in_vec.y, in_vec.z, 1.0};

    // Apply forward transform
    const vec4 res = Mat4_mul_vec4(trans.backwards, tmp);

    return (vec3){res.x, res.y, res.z};
}

// Function to extract the linear portion of the transform.
mat3 Tranform_Extract_Linear_Transform(Transform *const transform)
{
    mat3 linear = {0};

    // get mat3 from mat4
    linear.elements[0][1] = transform->forward.elements[0][1];
    linear.elements[0][2] = transform->forward.elements[0][2];
    linear.elements[0][3] = transform->forward.elements[0][3];

    linear.elements[1][1] = transform->forward.elements[1][1];
    linear.elements[1][2] = transform->forward.elements[1][2];
    linear.elements[1][3] = transform->forward.elements[1][3];

    linear.elements[2][1] = transform->forward.elements[2][1];
    linear.elements[2][2] = transform->forward.elements[2][2];
    linear.elements[2][3] = transform->forward.elements[2][3];

    // Invert and transpose.
    linear = mat3_inverse(linear);
    linear = mat3_transpose_to(linear);

    return linear;
}
vec3 Transform_Apply_Normal(const Transform trans, const vec3 normal)
{
    // Apply the transform and return the result.
    return mat3_mul_vec3(trans.linear, normal);
}

// direction = true = Forward
Ray Transform_Apply_Ray(Transform trans, Ray ray, bool direction)
{
    Ray ret;

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

// Transform Transform_Set(const vec3 translation, const vec3 rotation, const vec3 scale, const bool set_linear_transform)
Transform Transform_Set(const vec3 translation, const vec3 rotation, const vec3 scale)
{
    Transform res = {0};

    // TRANSLATION
    const Mat4 translation_matrix = Mat4_Make_Transform(translation);

    // ROTATION
    // Mat4 rotation_matrix = Mat4_Make_Rotation(rotation);
    const Mat4 z_rotation_matrix = Mat4_ZRot_Make(rotation.z);
    const Mat4 y_rotation_matrix = Mat4_YRot_Make(rotation.y);
    const Mat4 x_rotation_matrix = Mat4_XRot_Make(rotation.x);

    const Mat4 rotation_matrix = Mat4_Mul_Mat4(Mat4_Mul_Mat4(x_rotation_matrix, y_rotation_matrix), z_rotation_matrix);

    // SCALE
    const Mat4 scale_matrix = Mat4_Scale_Make(scale);

    res.forward   = Mat4_Mul_Mat4(Mat4_Mul_Mat4(translation_matrix, rotation_matrix), scale_matrix);
    res.backwards = Mat4_Inverse(res.forward);

    // if (set_linear_transform)
    //     res.linear = Tranform_Extract_Linear_Transform(&res);

    return res;
}