#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Lights.h"
#include "../Objects/Objects.h"

bool Light_Compute_Illumination(const PointLight light,
                                const Objects    objects,
                                const size_t     current_object_index,
                                const vec3       intPoint,
                                const vec3       localNormal,
                                vec3 *const      colour,
                                double *const    intensity);

#endif // __LIGHT_H__