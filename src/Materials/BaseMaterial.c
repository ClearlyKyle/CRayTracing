#include "Material.h"

#include "../Lights/Lights.h"

void Material_Set_Reflection_Ray_Count(const int value)
{
    Reflection_Ray_Count = value;
}

// BASE MATERIAL
vec3 Material_Base_Compute_Colour(Objects      objects,
                                  const Lights lights,
                                  const size_t current_object_index,
                                  vec3 *const  int_point,
                                  vec3 *const  local_normal,
                                  const Ray   *camera_ray)
{
    // Define an initial material color.
    // vec3 mat_colour = VEC3_INIT_ZERO;
    return VEC3_INIT_ZERO;
}

vec3 Material_Base_Compute_Diffuse_Colour(Objects      objects,
                                          const Lights lights,
                                          const size_t current_object_index,
                                          vec3 *const  int_point,
                                          vec3 *const  local_normal,
                                          const vec3   base_colour)
{
    // Compute the color due to diffuse illumination.
    vec3 diffuse_colour = VEC3_INIT_ZERO;
    vec3 colour         = VEC3_INIT_ZERO;

    double intensity  = 0.0;
    double red        = 0.0;
    double green      = 0.0;
    double blue       = 0.0;
    bool   illumFound = false;
    bool   validIllum = false;

    for (int current_light = 0; current_light < lights.count; current_light++)
    {
        switch (lights.lights[current_light].type)
        {
        case LIGHT_POINT:
            validIllum = Light_Compute_Illumination(lights.lights[current_light].object.pointLight,
                                                    objects,
                                                    current_object_index,
                                                    *int_point,
                                                    *local_normal,
                                                    &colour,
                                                    &intensity);
            break;

        default:
            fprintf(stderr, "THIS LIGHT IS NOT SUPPORTED : %d\n", lights.lights[current_light].type);
            // app.running = false;
            break;
        }

        if (validIllum)
        {
            illumFound = true;
            red += colour.x * intensity;
            green += colour.y * intensity;
            blue += colour.z * intensity;
        }
    }

    if (illumFound)
    {
        diffuse_colour.x = red * base_colour.x;
        diffuse_colour.y = green * base_colour.y;
        diffuse_colour.z = blue * base_colour.z;
    }
    else
    {
        diffuse_colour.x = Ambient_Colour.x * Ambient_Intensity * base_colour.x;
        diffuse_colour.y = Ambient_Colour.y * Ambient_Intensity * base_colour.y;
        diffuse_colour.z = Ambient_Colour.z * Ambient_Intensity * base_colour.z;
    }

    // Return the material color.
    return diffuse_colour;
}

vec3 Material_Base_Compute_Reflection_Colour(Objects      objects,
                                             const Lights lights,
                                             const size_t current_object_index,
                                             vec3 *const  int_point,
                                             vec3 *const  local_normal,
                                             const Ray   *incident_ray,
                                             const vec3   base_colour)
{
    // Compute the reflection vector.
    const vec3 reflection_vector = vec3_reflection(incident_ray->lab, *local_normal);

    // Construct the reflection ray.
    const Ray reflection_ray = Ray_Init(*int_point, vec3_add(*int_point, reflection_vector));

    /* Cast this ray into the scene and find the closest object that it intersects with. */
    size_t closest_object_index;
    vec3   closest_int_point    = VEC3_INIT_ZERO;
    vec3   closest_local_normal = VEC3_INIT_ZERO;
    vec3   closest_local_colour = VEC3_INIT_ZERO;

    const bool intersectionFound = Material_Cast_Ray(reflection_ray,
                                                     objects,
                                                     current_object_index,
                                                     &closest_object_index,
                                                     &closest_int_point,
                                                     &closest_local_normal,
                                                     &closest_local_colour);

    /* Compute illumination for closest object assuming that there was a valid intersection. */
    vec3 mat_colour = VEC3_INIT_ZERO;

    if ((intersectionFound) && (Reflection_Ray_Count < Max_Reflection_Rays))
    {
        // Increment the reflectionRayCount.
        Reflection_Ray_Count++;

        // Check if a material has been assigned.
        if (objects.shapes[closest_object_index].mat)
        {
            mat_colour = Material_Compute_Colour(objects,
                                                 lights,
                                                 closest_object_index,
                                                 &closest_int_point,
                                                 &closest_local_normal,
                                                 &reflection_ray);
        }
        else
        {
            mat_colour = Material_Base_Compute_Diffuse_Colour(objects,
                                                              lights,
                                                              closest_object_index,
                                                              &closest_int_point,
                                                              &closest_local_normal,
                                                              objects.shapes[closest_object_index].base_colour);
        }
    }
    else
    {
        // Leave mat_colour as it is.
    }

    return mat_colour;
}