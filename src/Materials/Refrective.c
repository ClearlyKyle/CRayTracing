#include "Material.h"

vec3 Refrective_Compute_Colour(const Material mat,
                               Objects        objects,
                               const Lights   lights,
                               const size_t   current_object_index,
                               vec3 *const    int_point,
                               vec3 *const    local_normal,
                               const Ray     *camera_ray)
{
    // Define the initial material colors.
    vec3 mat_colour = VEC3_INIT_ZERO;
    vec3 ref_colour = VEC3_INIT_ZERO;
    vec3 dif_colour = VEC3_INIT_ZERO;
    vec3 spc_colour = VEC3_INIT_ZERO;
    vec3 trn_colour = VEC3_INIT_ZERO;

    // Compute the diffuse component.
    if (mat.has_texture)
    {
        const vec4 texture_colour = Texture_Get_Colour(mat.texture, objects.shapes[current_object_index].uv_coordinates);
        dif_colour                = Material_Compute_Diffuse_Colour(objects, lights, current_object_index, int_point, local_normal, (vec3){texture_colour.x, texture_colour.y, texture_colour.z});
    }
    else
    {
        dif_colour = Material_Compute_Diffuse_Colour(objects, lights, current_object_index, int_point, local_normal, mat.base_colour);
    }
    // Compute the reflection component.
    if (mat.reflectivity > 0.0)
        ref_colour = Material_Compute_Reflection_Colour(mat, objects, lights, current_object_index, int_point, local_normal, camera_ray, mat.base_colour);

    // Combine reflection and diffuse components.
    mat_colour.x = (ref_colour.x * mat.reflectivity) + (dif_colour.x * (1.0 - mat.reflectivity));
    mat_colour.y = (ref_colour.y * mat.reflectivity) + (dif_colour.y * (1.0 - mat.reflectivity));
    mat_colour.z = (ref_colour.z * mat.reflectivity) + (dif_colour.z * (1.0 - mat.reflectivity));

    // Compute the refractive component.
    if (mat.translucency > 0.0)
        trn_colour = Refractive_Material_Compute_Translucency(objects, lights, current_object_index, int_point, local_normal, camera_ray);

    // And combine with the current color.
    mat_colour.x = (trn_colour.x * mat.translucency) + (mat_colour.x * (1.0 - mat.translucency));
    mat_colour.y = (trn_colour.y * mat.translucency) + (mat_colour.y * (1.0 - mat.translucency));
    mat_colour.z = (trn_colour.z * mat.translucency) + (mat_colour.z * (1.0 - mat.translucency));

    // And compute the specular component.
    if (mat.shininess > 0.0)
        spc_colour = Refractive_Material_Compute_Specular(objects, lights, int_point, local_normal, camera_ray);

    // Add the specular component to the final color.
    mat_colour = vec3_add(mat_colour, spc_colour);

    return mat_colour;
}

vec3 Refractive_Material_Compute_Translucency(Objects      objects,
                                              const Lights lights,
                                              const size_t current_object_index,
                                              vec3 *const  int_point,
                                              vec3 *const  local_normal,
                                              const Ray   *camera_ray)
{
    return (vec3){0.0, 0.0, 0.0};
}

vec3 Refractive_Material_Compute_Specular(Objects      objects,
                                          const Lights lights,
                                          vec3 *const  int_point,
                                          vec3 *const  local_normal,
                                          const Ray   *camera_ray)
{
    return (vec3){0.0, 0.0, 0.0};
}