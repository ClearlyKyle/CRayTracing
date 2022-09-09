#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <stdbool.h>

#include "../Ray.h"
#include "../Textures/Textures.h"

// Forward decleratoins
typedef struct Objects Objects;
typedef struct Lights  Lights;

enum Material_Type
{
    MATERIAL_BASE,
    MATERIAL_SIMPLE,
    MATERIAL_REFRACTIVE
};

typedef struct Material
{
    enum Material_Type type;
    vec3               base_colour;
    double             reflectivity;
    double             shininess;

    double translucency;
    double ior;

    Texture texture;
    bool    has_texture;
} Material;

// ----------------------------------------------------------------------
static const int Max_Reflection_Rays  = 3;
static int       Reflection_Ray_Count = 0;
// void             Material_Set_Reflection_Ray_Count(const int value);

// Compute Colour Function pointer
typedef vec3 (*Compute_Colour_fptr)(Objects objects, const Lights lights, const size_t current_object_index, vec3 *const int_point, vec3 *const local_normal, const Ray *camera_ray);

// Function to cast a ray into the scene.
bool Material_Cast_Ray(const Ray     cast_ray,
                       Objects       objects,
                       const size_t  current_object_index,
                       size_t *const closests_object_index,
                       vec3 *const   closests_int_point,
                       vec3 *const   closests_local_normal,
                       vec3 *const   closests_local_colour);

// Compute Colour --------------------------------------------------------
// Function to return the color of the material.
vec3 Material_Compute_Colour(Objects      objects,
                             const Lights lights,
                             const size_t current_object_index,
                             vec3 *const  int_point,
                             vec3 *const  local_normal,
                             const Ray   *camera_ray);
// BASE MATERIAL
vec3 Material_Base_Compute_Colour(Objects objects, const Lights lights, const size_t current_object_index, vec3 *const int_point, vec3 *const local_normal, const Ray *camera_ray);
vec3 Material_Base_Compute_Diffuse_Colour(Objects objects, const Lights lights, const size_t current_object_index, vec3 *const int_point, vec3 *const local_normal, const vec3 base_colour);
vec3 Material_Base_Compute_Reflection_Colour(Objects objects, const Lights lights, const size_t current_object_index, vec3 *const int_point, vec3 *const local_normal, const Ray *incident_ray, const vec3 base_colour);

// SIMPLE MATERIAL
vec3 Material_Simple_Compute_Colour(Objects objects, const Lights lights, const size_t current_object_index, vec3 *const int_point, vec3 *const local_normal, const Ray *camera_ray);

// REFRACTIVE MATERIAL
vec3 Material_Refrective_Compute_Colour(Objects objects, const Lights lights, const size_t current_object_index, vec3 *const int_point, vec3 *const local_normal, const Ray *camera_ray);

// List of Compute Colour Functions
const static Compute_Colour_fptr Compute_Colour_fptr_array[] = {
    Material_Base_Compute_Colour,
    Material_Simple_Compute_Colour,
    Material_Refrective_Compute_Colour,
};

#endif // __MATERIAL_H__