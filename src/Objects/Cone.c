#include "Objects.h"

bool Cone_Test_Intersection(
    const Ray_t ray,
    vec3 *const int_point,
    vec3 *const local_normal,
    vec3 *const local_colour)
{
    Transform transform   = {0};
    vec3      base_colour = {0};

    // Copy the ray and apply the backwards transform.
    const Ray_t bck_ray = Transform_Apply_Ray(transform, ray, false);

    // Copy the m_lab vector from bckRay and normalize it.
    const vec3 v = vec3_normalise(bck_ray.lab);

    // Get the start point of the line.
    const vec3 p = bck_ray.point1;

    // Compute a, b and c.
    const double a = pow(v.x, 2.0) + pow(v.y, 2.0) - pow(v.x, 2.0);
    const double b = 2 * (p.x * v.x + p.y * v.y - p.x * v.x);
    const double c = pow(p.x, 2.0) + pow(p.y, 2.0) - pow(p.x, 2.0);

    // Compute b^2 - 4ac.
    const double numSQRT = sqrtf(pow(b, 2.0) - 4 * a * c);

    vec3 poi[3];
    vec3 t = VEC3_INIT_ZERO;

    bool t1Valid = false, t2Valid = false, t3Valid = false;

    if (numSQRT > 0.0)
    {
        // Compute the values of t.
        t.x = (-b + numSQRT) / (2 * a);
        t.y = (-b - numSQRT) / (2 * a);

        // Compute the points of intersection.

        poi[0] = vec3_add(bck_ray.point1, vec3_mul_scal(v, t.x));
        poi[1] = vec3_add(bck_ray.point1, vec3_mul_scal(v, t.y));

        if ((t.x > 0.0) && (poi[0].z > 0.0) && (poi[0].z < 1.0))
        {
            t1Valid = true;
        }
        else
        {
            t1Valid = false;
            t.x     = 100e6;
        }

        if ((t.y > 0.0) && (poi[1].z > 0.0) && (poi[1].z < 1.0))
        {
            t2Valid = true;
        }
        else
        {
            t2Valid = false;
            t.y     = 100e6;
        }
    }
    else
    {
        t1Valid = false;
        t2Valid = false;
        t.x     = 100e6;
        t.y     = 100e6;
    }

    // And test the end cap.
    if (CloseEnough(v.z, 0.0))
    {
        t3Valid = false;
        t.z     = 100e6;
    }
    else
    {
        // Compute values for t.
        t.z = (bck_ray.point1.z - 1.0) / -v.z;

        // Compute points of intersection.
        poi[2] = vec3_add(bck_ray.point1, vec3_mul_scal(v, t.z));

        // Check if these are valid.
        if ((t.z > 0.0) && (sqrtf(pow(poi[2].x, 2.0) + pow(poi[2].y, 2.0)) < 1.0))
        {
            t3Valid = true;
        }
        else
        {
            t3Valid = false;
            t.z     = 100e6;
        }
    }

    // If no valid intersections found, then we can stop.
    if ((!t1Valid) && (!t2Valid) && (!t3Valid))
        return false;

    // Check for the smallest valid value of t.
    int    minIndex = 0;
    double minValue = 10e6;

    if (t.x < minValue)
    {
        minValue = t.x;
        minIndex = 0;
    }
    if (t.y < minValue)
    {
        minValue = t.y;
        minIndex = 1;
    }
    if (t.z < minValue)
    {
        minValue = t.z;
        minIndex = 2;
    }

    /* If minIndex is either 0 or 1, then we have a valid intersection with the cone itself. */
    vec3 valid_POI = poi[minIndex];
    if (minIndex < 2)
    {
        // Transform the intersection point back into world coordinates.
        *int_point = Transform_Apply_Forward(transform, valid_POI);

        // Compute the local normal.
        vec3       local_origin = {0.0, 0.0, 0.0};
        const vec3 globalOrigin = Transform_Apply_Forward(transform, local_origin);

        vec3 org_normal;
        org_normal.x = valid_POI.x;
        org_normal.y = valid_POI.y;
        org_normal.z = -sqrtf(pow(org_normal.x, 2.0) + pow(org_normal.y, 2.0));

        vec3 new_normal = vec3_sub(Transform_Apply_Forward(transform, org_normal), globalOrigin);
        new_normal      = vec3_normalise(new_normal);

        *local_normal = new_normal;
        *local_colour = base_colour;

        return true;
    }
    else
    {
        // Check the end cap.
        if (!CloseEnough(v.z, 0.0))
        {
            // Check if we are inside the disk.
            if (sqrtf(pow(valid_POI.x, 2.0) + pow(valid_POI.y, 2.0)) < 1.0)
            {
                // Transform the intersection point back into world coordinates.
                *int_point = Transform_Apply_Forward(transform, valid_POI);

                // Compute the local normal.
                vec3 local_origin  = {0.0, 0.0, 0.0};
                vec3 normal_vector = {0.0, 0.0, 1.0};

                const vec3 globalOrigin = Transform_Apply_Forward(transform, local_origin);

                vec3 new_normal = vec3_sub(Transform_Apply_Forward(transform, normal_vector), globalOrigin);
                new_normal      = vec3_normalise(new_normal);

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