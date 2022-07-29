#ifndef __SIMPLEMATERIAL_H__
#define __SIMPLEMATERIAL_H__

#include "vec3.h"
#include "Ray.h"
#include "Objects.h"
#include "Lights.h"

#include "Material.h"

struct SimpleMaterial
{
    vec3   base_colour; // 1.0, 0.0, 1.0
    double reflectivity;
    double shininess;
};

typedef struct SimpleMaterial SimpleMaterial;

inline SimpleMaterial SimpleMaterial_Init()
{
    return (SimpleMaterial){
        .base_colour  = {1.0, 0.0, 0.0},
        .reflectivity = 0.0,
        .shininess    = 0.0,
    };
}

// Function to return the color.
vec3 SimpleMaterial_Compute_Colour(const Material mat,
                                   const Objects  objects,
                                   const Lights   lights,
                                   const size_t   current_object_index,
                                   vec3 *const    int_point,
                                   vec3 *const    local_normal,
                                   const Ray_t   *camera_ray);

// Function to compute specular highlights.
vec3 SimpleMaterial_Compute_Specular(const Material mat,
                                     const Objects  objects,
                                     const Lights   lights,
                                     vec3 *const    int_point,
                                     vec3 *const    local_normal,
                                     const Ray_t   *camera_ray);

#endif // __SIMPLEMATERIAL_H__