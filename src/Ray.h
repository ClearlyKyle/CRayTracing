#ifndef __RAY_H__
#define __RAY_H__

#include "vec3.h"

typedef struct Ray
{
    vec3 point1;
    vec3 point2;
    vec3 lab;
} Ray;

inline Ray Ray_Init(const vec3 p1, const vec3 p2)
{
    return (Ray){
        .point1 = p1,
        .point2 = p2,
        .lab    = {p2.x - p1.x, p2.y - p1.y, p2.z - p1.z},
    };
}

#endif // __RAY_H__