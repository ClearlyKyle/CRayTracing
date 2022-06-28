#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <stdbool.h>
#include "vec3.h"

struct Light_s
{
    vec3   location;
    vec3   colour;
    double intensity;
};

typedef struct Light_s Light_t;

bool Light_Compute_Illumination(const Light_t *light, const vec3 intPoint, const vec3 localNormal, vec3 *colour, double *intensity);

// virtual bool ComputeIllumination(const qbVector<double> &intPoint, const qbVector<double> &localNormal,
//                                  const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList,
//                                  const std::shared_ptr<qbRT::ObjectBase> &currentObject,
//                                  qbVector<double> &color, double &intensity);

#endif // __LIGHT_H__