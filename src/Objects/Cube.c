#include "Objects.h"

bool Cube_Test_Intersection(Shape *const shape,
                            const Ray    ray,
                            vec3 *const  int_point,
                            vec3 *const  local_normal,
                            vec3 *const  local_colour)
{
    if (!shape->visible)
        return false;

    const Transform transform   = shape->transform;
    const vec3      base_colour = shape->base_colour;

    // Copy the ray and apply the backwards transform.
    const Ray bck_ray = Transform_Apply_Ray(transform, ray, false);

    // Extract values of a.
    const double ax = bck_ray.point1.x;
    const double ay = bck_ray.point1.y;
    const double az = bck_ray.point1.z;

    // Extract the value of k.
    const vec3   k  = vec3_normalise(bck_ray.lab);
    const double kx = k.x;
    const double ky = k.y;
    const double kz = k.z;

    // Test for intersections with each plane (side of the box).
    // Top and bottom.
    double t[6];
    double u[6];
    double v[6];
    if (!CLOSE_ENOUGH(kz, 0.0))
    {
        t[0] = (az - 1.0) / -kz;
        t[1] = (az + 1.0) / -kz;
        u[0] = ax + kx * t[0];
        v[0] = ay + ky * t[0];
        u[1] = ax + kx * t[1];
        v[1] = ay + ky * t[1];
    }
    else
    {
        t[0] = 100e6;
        t[1] = 100e6;
        u[0] = 0.0;
        v[0] = 0.0;
        u[1] = 0.0;
        v[1] = 0.0;
    }

    // Left and right.
    if (!CLOSE_ENOUGH(kx, 0.0))
    {
        t[2] = (ax + 1.0) / -kx;
        t[3] = (ax - 1.0) / -kx;
        u[2] = az + kz * t[2];
        v[2] = ay + ky * t[2];
        u[3] = az + kz * t[3];
        v[3] = ay + ky * t[3];
    }
    else
    {
        t[2] = 100e6;
        t[3] = 100e6;
        u[2] = 0.0;
        v[2] = 0.0;
        u[3] = 0.0;
        v[3] = 0.0;
    }

    // Front and back.
    if (!CLOSE_ENOUGH(ky, 0.0))
    {
        t[4] = (ay + 1.0) / -ky;
        t[5] = (ay - 1.0) / -ky;
        u[4] = ax + kx * t[4];
        v[4] = az + kz * t[4];
        u[5] = ax + kx * t[5];
        v[5] = az + kz * t[5];
    }
    else
    {
        t[4] = 100e6;
        t[5] = 100e6;
        u[4] = 0.0;
        v[4] = 0.0;
        u[5] = 0.0;
        v[5] = 0.0;
    }

    // Find the index of the smallest non-negative value of t.
    double final_u            = 0.0;
    double final_v            = 0.0;
    double final_t            = 100e6;
    int    final_index        = 0;
    bool   valid_intersection = false;
    for (int i = 0; i < 6; ++i)
    {
        if ((t[i] < final_t) && (t[i] > 0.0) && (fabs(u[i]) <= 1.0) && (fabs(v[i]) <= 1.0))
        {
            final_t            = t[i];
            final_index        = i;
            final_u            = u[i];
            final_v            = v[i];
            valid_intersection = true;
        }
    }

    if (valid_intersection)
    {
        // Compute the point of intersection.
        const vec3 poi = vec3_add(bck_ray.point1, vec3_mul_scal(k, final_t));

        // Compute the normal vector
        vec3 normal_vector = VEC3_INIT_ZERO;
        switch (final_index)
        {
        case 0:
            normal_vector = (vec3){0.0, 0.0, 1.0}; // Down.
            break;

        case 1:
            normal_vector = (vec3){0.0, 0.0, -1.0}; // Up.
            break;

        case 2:
            normal_vector = (vec3){-1.0, 0.0, 0.0}; // Left.
            break;

        case 3:
            normal_vector = (vec3){1.0, 0.0, 0.0}; // Right.
            break;

        case 4:
            normal_vector = (vec3){0.0, -1.0, 0.0}; // Backwards (towards the camera).
            break;

        case 5:
            normal_vector = (vec3){0.0, 1.0, 0.0}; // Forwards (away from the camera).
            break;
        }

        // Transform the intersection point back into world coordinates.
        *int_point = Transform_Apply_Forward(transform, poi);

        // Transform the normal vector.
        *local_normal = Transform_Apply_Normal(transform, normal_vector);
        *local_normal = vec3_normalise(*local_normal);

        // Return the base color.
        *local_colour = base_colour;

        // Store the (u,v) coordinates for possible later use.
        shape->uv_coordinates = (vec2){final_u, final_v};
        return true;
    }
    else
    {
        return false;
    }
}