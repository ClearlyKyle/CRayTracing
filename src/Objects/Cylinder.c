#include "Objects.h"

bool Cylinder_Test_Intersection(const Transform transform,
                                const vec3      base_colour,
                                const Ray_t     ray,
                                vec3 *const     int_point,
                                vec3 *const     local_normal,
                                vec3 *const     local_colour)
{
    // Copy the ray and apply the backwards transform.
    const Ray_t bck_ray = Transform_Apply_Ray(transform, ray, false);

    // Copy the m_lab vector from bckRay and normalize it.
    const vec3 v = vec3_normalise(bck_ray.lab);

    // Get the start point of the line.
    const vec3 p = bck_ray.point1;

    // Compute a, b and c.
    const double a = pow(v.x, 2.0) + pow(v.y, 2.0);
    const double b = 2.0 * (p.x * v.x + p.y * v.y);
    const double c = pow(p.x, 2.0) + pow(p.y, 2.0) - 1.0;

    // Compute b^2 - 4ac.
    const double numSQRT = sqrt(pow(b, 2.0) - 4.0 * a * c);

    // Test for intersections.
    // First with the cylinder itself.
    vec3   poi[4];
    double t[4];

    bool t1Valid = false, t2Valid = false, t3Valid = false, t4Valid = false;
    if (numSQRT > 0.0)
    {
        // There was an intersection.
        // Compute the values for t.
        t[0] = (-b + numSQRT) / (2 * a);
        t[1] = (-b - numSQRT) / (2 * a);

        // Compute the points of intersection.
        poi[0] = vec3_add(bck_ray.point1, vec3_mul_scal(v, t[0]));
        poi[1] = vec3_add(bck_ray.point1, vec3_mul_scal(v, t[1]));

        // Check if any of these are valid.
        if ((t[0] > 0.0) && (fabs(poi[0].z) < 1.0))
        {
            t1Valid = true;
        }
        else
        {
            t1Valid = false;
            t[0]    = 100e6;
        }

        if ((t[1] > 0.0) && (fabs(poi[1].z) < 1.0))
        {
            t2Valid = true;
        }
        else
        {
            t2Valid = false;
            t[1]    = 100e6;
        }
    }
    else
    {
        t1Valid = false;
        t2Valid = false;
        t[0]    = 100e6;
        t[1]    = 100e6;
    }

    // And test the end caps.
    if (CLOSE_ENOUGH(v.z, 0.0))
    {
        t3Valid = false;
        t4Valid = false;
        t[2]    = 100e6;
        t[3]    = 100e6;
    }
    else
    {
        // Compute the values of t.
        t[2] = (bck_ray.point1.z - 1.0) / -v.z;
        t[3] = (bck_ray.point1.z + 1.0) / -v.z;

        // Compute the points of intersection.
        poi[2] = vec3_add(bck_ray.point1, vec3_mul_scal(v, t[2]));
        poi[3] = vec3_add(bck_ray.point1, vec3_mul_scal(v, t[3]));

        // Check if these are valid.
        if ((t[2] > 0.0) && (sqrt(pow(poi[2].x, 2.0) + pow(poi[2].y, 2.0)) < 1.0))
        {
            t3Valid = true;
        }
        else
        {
            t3Valid = false;
            t[2]    = 100e6;
        }

        if ((t[3] > 0.0) && (sqrt(pow(poi[3].x, 2.0) + pow(poi[3].y, 2.0)) < 1.0))
        {
            t4Valid = true;
        }
        else
        {
            t4Valid = false;
            t[3]    = 100e6;
        }
    }

    // If no valid intersections found, the we can stop.
    if ((!t1Valid) && (!t2Valid) && (!t3Valid) && (!t4Valid))
        return false;

    // Check for the smallest valid value of t.
    int    minIndex = 0;
    double minValue = 10e6;

    for (int i = 0; i < 4; ++i)
    {
        if (t[i] < minValue)
        {
            minValue = t[i];
            minIndex = i;
        }
    }

    /* If minIndex is either 0 or 1, then we have a valid intersection with the cylinder itself. */
    vec3 valid_POI = poi[minIndex];
    if (minIndex < 2)
    {
        // Transform the intersection point back into world coordinates.
        *int_point = Transform_Apply_Forward(transform, valid_POI);

        // Compute the local normal.
        vec3       local_origin = {0.0, 0.0, 0.0};
        const vec3 globalOrigin = Transform_Apply_Forward(transform, local_origin);

        const vec3 org_normal = {valid_POI.x, valid_POI.y, 0.0};
        vec3       new_normal = vec3_sub(Transform_Apply_Forward(transform, org_normal), globalOrigin);
        new_normal            = vec3_normalise(new_normal);

        *local_normal = new_normal;
        *local_colour = base_colour;

        return true;
    }
    else
    {
        // Otherwise check the end caps.
        if (!CLOSE_ENOUGH(v.z, 0.0))
        {
            // Check if we are inside the disk.
            if (sqrt(pow(valid_POI.x, 2.0) + pow(valid_POI.y, 2.0)) < 1.0)
            {
                // Transform the intersection point back into world coordinates.
                *int_point = Transform_Apply_Forward(transform, valid_POI);

                // Compute the local normal.
                vec3 local_origin  = {0.0, 0.0, 0.0};
                vec3 normal_vector = {0.0, 0.0, 0.0 + valid_POI.z};

                const vec3 globalOrigin = Transform_Apply_Forward(transform, local_origin);

                vec3 new_normal = vec3_sub(Transform_Apply_Forward(transform, normal_vector), globalOrigin);
                new_normal      = vec3_normalise(new_normal);

                *local_normal = new_normal;
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