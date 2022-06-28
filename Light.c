#include "Light.h"

bool Light_Compute_Illumination(const Light_t *light, const vec3 intPoint, const vec3 localNormal, vec3 *colour, double *intensity)
{
    // Construct a vector pointing from the intersection point to the light.
    const vec3 light_direction = vec3_normalise(vec3_sub(light->location, intPoint));

    // Compute the angle between the local normal and the light ray.
    // Note that we assume that localNormal is a unit vector.
    const double angle = acos(vec3_dot(localNormal, light_direction));

    // If the normal is pointing away from the light, then we have no illumination.
    if (angle > 1.5708)
    {
        // No illumination.
        *colour    = light->colour;
        *intensity = 0.0;
        return false;
    }
    else
    {
        // We do have illumination.
        *colour    = light->colour;
        *intensity = light->intensity * (1.0 - (angle / 1.5708));
        // intensity = 1.0;
        return true;
    }

    return true;
}
