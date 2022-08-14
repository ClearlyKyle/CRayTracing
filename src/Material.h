#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "Ray.h"
#include "Lights/Lights.h"

void Material_Set_Reflection_Ray_Count(const int value);

// Function to return the color of the material.
vec3 Material_Compute_Colour(const Objects objects,
                             const Lights  lights,
                             const size_t  current_object_index,
                             vec3 *const   int_point,
                             vec3 *const   local_normal,
                             const Ray  *camera_ray);

// Function to compute diffuse color.
vec3 Material_Compute_Diffuse_Colour(const Objects objects,
                                     const Lights  lights,
                                     const size_t  current_object_index,
                                     vec3 *const   int_point,
                                     vec3 *const   local_normal,
                                     const vec3    base_colour);

// Function to compute the reflection color.
vec3 Material_Compute_Reflection_Colour(const Material mat,
                                        const Objects  objects,
                                        const Lights   lights,
                                        const size_t   current_object_index,
                                        vec3 *const    int_point,
                                        vec3 *const    local_normal,
                                        const Ray   *incident_ray,
                                        const vec3     base_colour);

// Function to cast a ray into the scene.
bool Material_Cast_Ray(const Ray   cast_ray,
                       const Objects objects,
                       const size_t  current_object_index,
                       size_t *const closests_object_index,
                       vec3 *const   closests_int_point,
                       vec3 *const   closests_local_normal,
                       vec3 *const   closests_local_colour);

typedef struct SimpleMaterial
{
    vec3   base_colour; // 1.0, 0.0, 1.0
    double reflectivity;
    double shininess;
} SimpleMaterial;

inline SimpleMaterial SimpleMaterial_Init()
{
    return (SimpleMaterial){
        .base_colour  = {1.0, 0.0, 0.0},
        .reflectivity = 0.0,
        .shininess    = 0.0,
    };
}

// Function to return the color.
vec3 Simple_Material_Compute_Colour(const Material mat,
                                    const Objects  objects,
                                    const Lights   lights,
                                    const size_t   current_object_index,
                                    vec3 *const    int_point,
                                    vec3 *const    local_normal,
                                    const Ray   *camera_ray);

// Function to compute specular highlights.
vec3 SimpleMaterial_Compute_Specular(const Material mat,
                                     const Objects  objects,
                                     const Lights   lights,
                                     vec3 *const    int_point,
                                     vec3 *const    local_normal,
                                     const Ray   *camera_ray);

#endif // __MATERIAL_H__