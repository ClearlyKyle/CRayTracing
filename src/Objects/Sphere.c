#include "Objects.h"

#include <math.h>

bool Sphere_Test_Intersection(Shape *const shape,
                              const Ray    ray,
                              vec3 *const  int_point,
                              vec3 *const  local_normal,
                              vec3 *const  local_colour)
{
    if (!shape->visible)
        return false;

    const Transform transform   = shape->transform;
    const vec3      base_colour = shape->base_colour;

    const Ray back_ray = Transform_Apply_Ray(transform, ray, false);

    // Compute the values of a, b and c.
    const vec3 vhat = vec3_normalise(back_ray.lab);

    /* Note that a is equal to the squared magnitude of the
            direction of the cast ray. As this will be a unit vector,
            we can conclude that the value of 'a' will always be 1. */
    // a = 1.0;

    // Calculate b.
    const double b = 2.0 * vec3_dot(back_ray.point1, vhat);

    // Calculate c.
    const double c = vec3_dot(back_ray.point1, back_ray.point1) - 1.0;

    // Test whether we actually have an intersection.
    // Qudratic formula...
    const double intTest = (b * b) - 4.0 * c;

    vec3 poi;
    if (intTest > 0.0)
    {
        const double numSQRT = sqrt(intTest);
        const double t1      = (-b + numSQRT) / 2.0;
        const double t2      = (-b - numSQRT) / 2.0;

        /* If either t1 or t2 are negative, then at least part of the object is
                behind the camera and so we will ignore it. */
        if ((t1 < 0.0) && (t2 < 0.0))
        {
            return false;
        }
        else
        {
            // Determine which point of intersection was closest to the camera.
            if (t1 < t2)
            {
                if (t1 > 0.0)
                {
                    poi = vec3_add(back_ray.point1, vec3_mul_scal(vhat, t1));
                }
                else
                {
                    if (t2 > 0.0)
                    {
                        poi = vec3_add(back_ray.point1, vec3_mul_scal(vhat, t2));
                    }
                    else
                    {
                        return false;
                    }
                }
            }
            else
            {
                if (t2 > 0.0)
                {
                    poi = vec3_add(back_ray.point1, vec3_mul_scal(vhat, t2));
                }
                else
                {
                    if (t1 > 0.0)
                    {
                        poi = vec3_add(back_ray.point1, vec3_mul_scal(vhat, t1));
                    }
                    else
                    {
                        return false;
                    }
                }
            }

            // Transform the intersection point back into world coordinates.
            *int_point = Transform_Apply_Forward(transform, poi);

            // Compute the local normal (easy for a sphere at the origin!).
            *local_normal = Transform_Apply_Normal(transform, poi);
            *local_normal = vec3_normalise(*local_normal);

            // Return the base color.
            *local_colour = base_colour;

            // UV Coordinates
            const double x = poi.x;
            const double y = poi.y;
            const double z = poi.z;

            const double one_over_pie = 1.0 / M_PI;
            const double u            = atan2(sqrt(pow(x, 2.0) + pow(y, 2.0)), z) * one_over_pie;
            const double v            = atan2(y, x) * one_over_pie;

            // u /= M_PI;
            // v /= M_PI;

            shape->uv_coordinates = (vec2){u, v};
        }
        return true;
    }
    else
    {
        return false;
    }
}
