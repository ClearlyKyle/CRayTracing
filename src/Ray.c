#include "Ray.h"

Ray_t Ray_Init(const vec3 p1, const vec3 p2)
{
    Ray_t ray = {
        .point1 = p1,
        .point2 = p2,
        .lab    = {p2.x - p1.x, p2.y - p1.y, p2.z - p1.z},
    };

    return ray;
}
