#include "Material.h"

#include "../Lights/Lights.h"

vec3 Material_Compute_Colour(Objects      objects,
                             const Lights lights,
                             const size_t current_object_index,
                             vec3 *const  int_point,
                             vec3 *const  local_normal,
                             const Ray   *camera_ray)
{
    const enum Matrial_type current_material_type = objects.shapes[current_object_index].mat->type;

    return Compute_Colour_fptr_array[current_material_type](objects,
                                                            lights,
                                                            current_object_index,
                                                            int_point,
                                                            local_normal,
                                                            camera_ray);
}

bool Material_Cast_Ray(const Ray     cast_ray,
                       Objects       objects,
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
        if (i != current_object_index)
        {
            const bool valid_intersection = Object_Test_Intersection(&objects.shapes[i],
                                                                     cast_ray,
                                                                     &int_point,
                                                                     &local_normal,
                                                                     &local_colour);

            if (valid_intersection)
            {
                intersection_found = true;

                // Compute the distance between the camera and the point of intersection.
                const double dist = vec3_length(vec3_sub(int_point, cast_ray.point1));

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
    }

    return intersection_found;
}
