#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "./Maths/vec3.h"

#include "Ray.h"

typedef struct Camera
{
    vec3 position;
    vec3 lookat;
    vec3 up;

    double length;
    double horizontal_size;
    double aspect_ratio;

    vec3 alignment_vector;
    vec3 projection_screen_U;
    vec3 projection_screen_V;
    vec3 projection_screen_center;
} Camera_t;

Camera_t Camera_Init();
void     Camera_Update_Geometry(Camera_t *cam);
Ray      Camera_Generate_Ray(const Camera_t *cam, const double screen_x, const double screen_y);

#endif // __CAMERA_H__