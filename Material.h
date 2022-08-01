#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "Ray.h"
#include "Lights/Lights.h"
#include "Objects/Objects.h"

void inline Material_Set_Reflection_Ray_Count(const int value);

// Function to return the color of the material.
vec3 Material_Compute_Colour(const Objects objects,
                             const Lights  lights,
                             const size_t  current_object_index,
                             vec3 *const   int_point,
                             vec3 *const   local_normal,
                             const Ray_t  *camera_ray);

// Function to compute diffuse color.
vec3 Material_Compute_Diffuse_Colour(const Objects objects,
                                     const Lights  lights,
                                     const size_t  current_object_index,
                                     vec3 *const   int_point,
                                     vec3 *const   local_normal,
                                     const Ray_t  *camera_ray,
                                     const vec3    base_colour);

// Function to compute the reflection color.
vec3 Material_Compute_Reflection_Colour(const Material mat,
                                        const Objects  objects,
                                        const Lights   lights,
                                        const size_t   current_object_index,
                                        vec3 *const    int_point,
                                        vec3 *const    local_normal,
                                        const Ray_t   *incident_ray,
                                        const vec3     base_colour);

// Function to cast a ray into the scene.
bool Material_Cast_Ray(Ray_t *const  cast_ray,
                       const Objects objects,
                       const size_t  current_object_index,
                       size_t *const closests_object_index,
                       vec3 *const   closests_int_point,
                       vec3 *const   closests_local_normal,
                       vec3 *const   closests_local_colour);

#endif // __MATERIAL_H__