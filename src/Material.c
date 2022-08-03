#include "Material.h"

static int Max_Reflection_Rays;
static int Reflection_Ray_Count;

void Material_Set_Reflection_Ray_Count(const int value)
{
    Reflection_Ray_Count = value;
}

vec3 Material_Compute_Colour(const Objects objects,
                             const Lights  lights,
                             const size_t  current_object_index,
                             vec3 *const   int_point,
                             vec3 *const   local_normal,
                             const Ray_t  *camera_ray)
{
    // Define an initial material color.
    vec3 mat_colour = VEC3_INIT_ZERO;
    return mat_colour;
}

vec3 Material_Compute_Diffuse_Colour(const Objects objects,
                                     const Lights  lights,
                                     const size_t  current_object_index,
                                     vec3 *const   int_point,
                                     vec3 *const   local_normal,
                                     const Ray_t  *camera_ray,
                                     const vec3    base_colour)
{
    // Compute the color due to diffuse illumination.
    vec3 diffuse_colour = VEC3_INIT_ZERO;
    vec3 colour         = VEC3_INIT_ZERO;

    double intensity  = 0.0;
    double red        = 0.0;
    double green      = 0.0;
    double blue       = 0.0;
    bool   validIllum = false;
    bool   illumFound = false;

    for (int current_object_index = 0; current_object_index < lights.count; current_object_index++)
    {
        switch (lights.lights[current_object_index].type)
        {

        case LIGHT_POINT:
            validIllum = Light_Compute_Illumination(lights.lights[current_object_index].object.pointLight,
                                                    objects,
                                                    current_object_index,
                                                    *int_point,
                                                    *local_normal,
                                                    &colour,
                                                    &intensity);
            break;

        default:
            fprintf(stderr, "THIS LIGHT IS NOT SUPPORTED : %d\n", lights.lights[current_object_index].type);
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

    // Return the material color.
    return diffuse_colour;
}

vec3 Material_Compute_Reflection_Colour(const Material mat,
                                        const Objects  objects,
                                        const Lights   lights,
                                        const size_t   current_object_index,
                                        vec3 *const    int_point,
                                        vec3 *const    local_normal,
                                        const Ray_t   *incident_ray,
                                        const vec3     base_colour)
{
    // vec3 reflection_colour = VEC3_INIT_ZERO;

    // Compute the reflection vector.
    const vec3 reflection_vector = vec3_reflection(incident_ray->lab, *local_normal);

    // Construct the reflection ray.
    Ray_t reflection_ray = Ray_Init(*int_point, vec3_add(*int_point, reflection_vector));

    /* Cast this ray into the scene and find the closest object that it intersects with. */
    size_t closest_object_index;
    vec3   closest_int_point    = VEC3_INIT_ZERO;
    vec3   closest_local_normal = VEC3_INIT_ZERO;
    vec3   closest_local_colour = VEC3_INIT_ZERO;

    const bool intersectionFound = Material_Cast_Ray(&reflection_ray,
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
        // TODO : Check this has a material
        if (current_object_index)
        {
            // Use the material to compute the color.
            mat_colour = Material_Compute_Colour(
                objects,
                lights,
                current_object_index,
                &closest_int_point,
                &closest_local_normal,
                &reflection_ray);
        }
        else
        {
            mat_colour = Material_Compute_Diffuse_Colour(
                objects,
                lights,
                current_object_index,
                &closest_int_point,
                &closest_local_normal,
                incident_ray,
                base_colour);
        }
    }
    else
    {
        // Leave matColor as it is.
    }

    return mat_colour;
}

bool Material_Cast_Ray(Ray_t *const  cast_ray,
                       const Objects objects,
                       const size_t  current_object_index,
                       size_t *const closests_object_index,
                       vec3 *const   closests_int_point,
                       vec3 *const   closests_local_normal,
                       vec3 *const   closests_local_colour)
{
    // Test for intersections with all of the objects in the scene.
    vec3 int_point    = VEC3_INIT_ZERO;
    vec3 local_normal = VEC3_INIT_ZERO;
    vec3 local_colour = VEC3_INIT_ZERO;

    double minDist            = 1e6;
    bool   intersection_found = false;

    for (size_t i = 0; i < objects.count; i++)
    {
        bool validInt = false;

        switch (objects.shapes[i].type)
        {
        case SHAPE_SPHERE:
            validInt = Sphere_Test_Intersection(objects.shapes[i].object.sphere, cast_ray, &int_point, &local_normal, &local_colour);
            break;

        case SHAPE_PLANE:
            validInt = Plane_Test_Intersection(objects.shapes[i].object.plane, cast_ray, &int_point, &local_normal, &local_colour);
            break;

        default:
            fprintf(stderr, "THIS SHAPE IS NOT SUPPORTED : %d\n", objects.shapes[i].type);
            break;
        }

        if (validInt)
        {
            intersection_found = true;

            // Compute the distance between the camera and the point of intersection.
            const double dist = vec3_length(vec3_sub(int_point, cast_ray->point1));

            if (dist < minDist)
            {
                *closests_object_index = i;
                minDist                = dist;
                *closests_int_point    = int_point;
                *closests_local_normal = local_normal;
                *closests_local_colour = local_colour;
            }
        }
    }

    return intersection_found;
}
