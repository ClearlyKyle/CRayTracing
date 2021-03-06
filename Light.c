#include "Light.h"

bool Light_Compute_Illumination(const Light_t *light, const size_t current_object, const ShapeArray shapes[], const size_t num_shapes, const vec3 intPoint, const vec3 localNormal, vec3 *colour, double *intensity)
{
    // Construct a vector pointing from the intersection point to the light.
    const vec3 light_direction = vec3_normalise(vec3_sub(light->location, intPoint));

    // Compute a starting point.
    const vec3 start_point = intPoint;

    // Construct a ray from the point of intersection to the light.
    const Ray_t lightRay = Ray_Init(start_point, vec3_add(start_point, light_direction));

    /* Check for intersections with all of the objects in the scene, except for the current one. */
    vec3 poi       = VEC3_INIT_ZERO;
    vec3 poiNormal = VEC3_INIT_ZERO;
    vec3 poiColor  = VEC3_INIT_ZERO;
    bool validInt  = false;

    for (size_t i = 0; i < num_shapes; i++)
    {
        if (i != current_object)
        {
            switch (shapes[i].type)
            {
            case SHAPE_SPHERE:
                validInt = Sphere_Test_Intersection(shapes[i].object.sphere, &lightRay, &poi, &poiNormal, &poiColor);
                break;

            case SHAPE_PLANE:
                validInt = Plane_Test_Intersection(shapes[i].object.plane, &lightRay, &poi, &poiNormal, &poiColor);
                break;

            default:
                fprintf(stderr, "THIS SHAPE IS NOT SUPPORTED : %d\n", shapes[i].type);
                // app.running = false;
                break;
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
            *colour    = light->colour;
            *intensity = 0.0;
            return false;
        }
        else
        {
            //        // We do have illumination.
            *colour    = light->colour;
            *intensity = light->intensity * (1.0 - (angle / 1.5708));
            // intensity = 1.0;
            return true;
        }
    }
    else
    {
        // Shadow, so no illumination.
        *colour    = light->colour;
        *intensity = 0.0;
        return false;
    }
}