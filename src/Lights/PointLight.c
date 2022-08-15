#include "Lights.h"

bool Light_Compute_Illumination(const PointLight light,
                                const Objects    objects,
                                const size_t     current_object_index,
                                const vec3       intPoint,
                                const vec3       localNormal,
                                vec3 *const      colour,
                                double *const    intensity)
{
    // Construct a vector pointing from the intersection point to the light.
    const vec3   light_direction = vec3_normalise(vec3_sub(light.location, intPoint));
    const double light_dist      = vec3_length(vec3_sub(light.location, intPoint));

    // Compute a starting point.
    const vec3 start_point = intPoint;

    // Construct a ray from the point of intersection to the light.
    const Ray lightRay = Ray_Init(start_point, vec3_add(start_point, light_direction));

    /* Check for intersections with all of the objects in the scene, except for the current one. */
    vec3 poi        = VEC3_INIT_ZERO;
    vec3 poi_normal = VEC3_INIT_ZERO;
    vec3 poi_colour = VEC3_INIT_ZERO;
    bool validInt   = false;

    for (size_t i = 0; i < objects.count; i++)
    {
        if (i != current_object_index)
        {
            validInt = objects.shapes[i].Test_Intersection(objects.shapes[i].transform,
                                                           objects.shapes[i].mat->base_colour,
                                                           lightRay,
                                                           &poi,
                                                           &poi_normal,
                                                           &poi_colour);

            if (validInt)
            {
                const double dist = vec3_length(vec3_sub(poi, start_point));
                if (dist > light_dist)
                    validInt = false;
            }
        }

        // If we have an intersection, then there is no point checking further so we can break out of the loop. In other words, this object is
        // blocking light from this light source
        if (validInt)
        {
            break;
        }
    }

    // Only continue to compute illumination if the light ray didn't intersect with any objects in the scene. Ie. no objects are
    // casting a shadow from this light source
    if (!validInt)
    {
        // Compute the angle between the local normal and the light ray.
        // Note that we assume that localNormal is a unit vector.
        const double angle = acos(vec3_dot(localNormal, light_direction));

        // If the normal is pointing away from the light, then we have no illumination.
        if (angle > 1.5708)
        {
            //        // No illumination.
            *colour    = light.colour;
            *intensity = 0.0;
            return false;
        }
        else
        {
            //        // We do have illumination.
            *colour    = light.colour;
            *intensity = light.intensity * (1.0 - (angle / 1.5708));
            return true;
        }
    }
    else
    {
        // Shadow, so no illumination.
        *colour    = light.colour;
        *intensity = 0.0;
        return false;
    }
}