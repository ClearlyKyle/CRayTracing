#ifndef __RAY_H__
#define __RAY_H__

#include "vec3.h"

typedef struct Ray
{
    vec3 point1;
    vec3 point2;
    vec3 lab;
} Ray_t;

Ray_t Ray_Init(const vec3 p1, const vec3 p2);

#endif // __RAY_H__