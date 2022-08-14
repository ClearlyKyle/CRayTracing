#include "Objects.h"

bool Plane_Test_Intersection(const Transform transform,
                             const vec3      base_colour,
                             const Ray     ray,
                             vec3 *const     int_point,
                             vec3 *const     local_normal,
                             vec3 *const     local_colour)
{
    // Copy the ray and apply the backwards transform.
    const Ray back_ray = Transform_Apply_Ray(transform, ray, false);

    // Copy the m_lab vector from bckRay and normalize it.
    const vec3 k = vec3_normalise(back_ray.lab);

    // Check if there is an intersection, ie. if the castRay is not parallel to the plane
    // if (!CloseEnough(k.GetElement(2), 0.0))
    if (!CLOSE_ENOUGH(k.z, 0.0))
    {
        // There is an intersection.
        const double t = back_ray.point1.z / -k.z;

        // If t is negative, then the intersection point must be behind the camera and we can ignore it.
        if (t > 0.0)
        {
            // Compute the values for u and v.
            const double u = back_ray.point1.x + (k.x * t);
            const double v = back_ray.point1.y + (k.y * t);

            // If the magnitude of both u and v is less than or equal to one then we must be in the plane.
            if ((fabs(u) < 1.0) && (fabs(v) < 1.0))
            {
                // Compute the point of intersection.
                const vec3 poi = vec3_add(back_ray.point1, vec3_mul_scal(k, t));

                // Transform the intersection point back into world coordinates.
                *int_point = Transform_Apply_Forward(transform, poi);

                // Compute the local normal.
                const vec3 localOrigin  = {0.0, 0.0, 0.0};
                const vec3 normalVector = {0.0, 0.0, -1.0};

                const vec3 globalOrigin = Transform_Apply_Forward(transform, localOrigin);
                *local_normal           = vec3_sub(Transform_Apply_Forward(transform, normalVector), globalOrigin);
                *local_normal           = vec3_normalise(*local_normal);

                // Return the base color.
                *local_colour = base_colour;

                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    return false;
}
