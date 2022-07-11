#include "Sphere.h"

#include <math.h>

bool Sphere_Test_Intersection(const Sphere sphere,
                              const Ray_t *ray,
                              vec3        *int_point,
                              vec3        *local_normal,
                              vec3        *local_colour)
{
    const Ray_t back_ray = Transform_Apply_Ray(sphere.transform, *ray, false);

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

    if (intTest > 0.0)
    {
        const double numSQRT = sqrt(intTest);
        const double t1      = (-b + numSQRT) / 2.0;
        const double t2      = (-b - numSQRT) / 2.0;

        /* If either t1 or t2 are negative, then at least part of the object is
                behind the camera and so we will ignore it. */
        if ((t1 < 0.0) || (t2 < 0.0))
        {
            return false;
        }
        else
        {
            // Determine which point of intersection was closest to the camera.
            if (t1 < t2)
            {
                *int_point = vec3_add(back_ray.point1, vec3_mul_scal(vhat, t1));
            }
            else
            {
                *int_point = vec3_add(back_ray.point1, vec3_mul_scal(vhat, t2));
            }

            // Compute the local normal (easy for a sphere at the origin!).
            *local_normal = *int_point;
            NORMALISE_VEC3(*local_normal);

            // Return the base Colour
            *local_colour = sphere.colour;
        }
        return true;
    }
    else
    {
        return false;
    }
}
