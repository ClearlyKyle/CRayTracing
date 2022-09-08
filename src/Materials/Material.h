#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <stdbool.h>

#include "../Ray.h"
#include "../Textures/Textures.h"

typedef struct Objects Objects;
typedef struct Lights  Lights;

enum Material_Type
{
    MATERIAL_BASE,
    MATERIAL_SIMPLE,
    MATERIAL_REFRACTIVE
};

// typedef struct SimpleMaterial
//{
//     vec3   base_colour;
//     double reflectivity;
//     double shininess;
// } SimpleMaterial;

// typedef struct Refractive
//{
//     vec3   base_colour;
//     double reflectivity;

//} Refractive;

typedef struct Material
{
    enum Material_Type type;
    vec3               base_colour;
    double             reflectivity;
    double             shininess;

    double translucency;
    double ior;

    // union
    //{
    //     struct refractive
    //     {
    //     };
    // };

    Texture texture;
    bool    has_texture;
} Material;

// -----------------------------------------------------------------------

void Material_Set_Reflection_Ray_Count(const int value);

// Function to return the color of the material.
// vec3 Material_Compute_Colour(Objects      objects,
//                             const Lights lights,
//                             const size_t current_object_index,
//                             vec3 *const  int_point,
//                             vec3 *const  local_normal,
//                             const Ray   *camera_ray);

// Function to compute diffuse color.
vec3 Material_Compute_Diffuse_Colour(Objects      objects,
                                     const Lights lights,
                                     const size_t current_object_index,
                                     vec3 *const  int_point,
                                     vec3 *const  local_normal,
                                     const vec3   base_colour);

// Function to compute the reflection color.
vec3 Material_Compute_Reflection_Colour(const Material mat,
                                        Objects        objects,
                                        const Lights   lights,
                                        const size_t   current_object_index,
                                        vec3 *const    int_point,
                                        vec3 *const    local_normal,
                                        const Ray     *incident_ray,
                                        const vec3     base_colour);

// Function to cast a ray into the scene.
bool Material_Cast_Ray(const Ray     cast_ray,
                       Objects       objects,
                       const size_t  current_object_index,
                       size_t *const closests_object_index,
                       vec3 *const   closests_int_point,
                       vec3 *const   closests_local_normal,
                       vec3 *const   closests_local_colour);

// Function to return the color.
vec3 Simple_Material_Compute_Colour(const Material mat,
                                    Objects        objects,
                                    const Lights   lights,
                                    const size_t   current_object_index,
                                    vec3 *const    int_point,
                                    vec3 *const    local_normal,
                                    const Ray     *camera_ray);

// Function to compute specular highlights.
vec3 SimpleMaterial_Compute_Specular(const Material mat,
                                     Objects        objects,
                                     const Lights   lights,
                                     vec3 *const    int_point,
                                     vec3 *const    local_normal,
                                     const Ray     *camera_ray);

vec3 Refrective_Compute_Colour(const Material mat,
                               Objects        objects,
                               const Lights   lights,
                               const size_t   current_object_index,
                               vec3 *const    int_point,
                               vec3 *const    local_normal,
                               const Ray     *camera_ray);

vec3 Refractive_Material_Compute_Translucency(Objects      objects,
                                              const Lights lights,
                                              const size_t current_object_index,
                                              vec3 *const  int_point,
                                              vec3 *const  local_normal,
                                              const Ray   *camera_ray);

vec3 Refractive_Material_Compute_Specular(const Material mat,
                                          Objects        objects,
                                          const Lights   lights,
                                          vec3 *const    int_point,
                                          vec3 *const    local_normal,
                                          const Ray     *camera_ray);

// Compute Colour --------------------------------------------------------

// Function pointer
typedef vec3 (*Compute_Colour_fptr)(const Material mat,
                                    Objects        objects,
                                    const Lights   lights,
                                    const size_t   current_object_index,
                                    vec3 *const    int_point,
                                    vec3 *const    local_normal,
                                    const Ray     *camera_ray);

vec3 Material_Compute_Colour(Objects      objects,
                             const Lights lights,
                             const size_t current_object_index,
                             vec3 *const  int_point,
                             vec3 *const  local_normal,
                             const Ray   *camera_ray);

#endif // __MATERIAL_H__