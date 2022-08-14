#ifndef __SINGLE_CYLINDER_H__
#define __SINGLE_CYLINDER_H__

#include "Scenes.h"

inline Scene Single_Cylinder(void)
{
    Scene scene;

    // Setup Lights
    scene.lights.count  = 3;
    scene.lights.lights = (LightArray *)malloc(sizeof(LightArray) * scene.lights.count);

    scene.lights.lights[0] = (LightArray){.type = LIGHT_POINT, .object.pointLight = {.location = {5.0, -10.0, -5.0}, .colour = {0.0, 0.0, 1.0}, .intensity = 1.0}};
    scene.lights.lights[1] = (LightArray){.type = LIGHT_POINT, .object.pointLight = {.location = {-5.0, -10.0, -5.0}, .colour = {1.0, 0.0, 1.0}, .intensity = 1.0}};
    scene.lights.lights[2] = (LightArray){.type = LIGHT_POINT, .object.pointLight = {.location = {0.0, -10.0, -5.0}, .colour = {0.0, 1.0, 1.0}, .intensity = 1.0}};

    // Setup Camera
    scene.cam = (Camera_t){
        .position        = {0.0, -10.0, -2.0},
        .lookat          = {0.0, 0.0, 0.0},
        .up              = {0.0, 0.0, 1.0},
        .length          = 1.0,
        .horizontal_size = 0.25,
        .aspect_ratio    = 1.0, // 800x800
    };
    Camera_Update_Geometry(&scene.cam);

    // Materials
    scene.mats_count = 2;
    scene.mats       = (Material *)malloc(sizeof(Material) * scene.mats_count);
    scene.mats[0]    = (Material){.base_colour = {0.2, 0.2, 0.8}, .reflectivity = 0.05, .shininess = 5.0}; // Blue Diffuse
    scene.mats[1]    = (Material){.base_colour = {1.0, 1.0, 1.0}, .reflectivity = 0.5, .shininess = 0.0};  // Floor

    // Setup Objects (Spheres, Planes)
    scene.objects = Objects_Init(2);

    scene.objects.shapes[0] = (Shape){.type              = SHAPE_PLANE,
                                      .mat               = &scene.mats[0],
                                      .base_colour       = {1.0, 1.0, 1.0},
                                      .transform         = Transform_Set((vec3){0.0, 0.0, 1.0}, (vec3){0.0, 0.0, 0.0}, (vec3){16.0, 16.0, 1.0}),
                                      .Test_Intersection = &Plane_Test_Intersection};

    scene.objects.shapes[1] = (Shape){.type              = SHAPE_CYLINDER,
                                      .mat               = &scene.mats[1],
                                      .base_colour       = {1.0, 1.0, 1.0},
                                      .transform         = Transform_Set((vec3){0.0, 0.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){1.0, 1.0, 1.0}),
                                      .Test_Intersection = &Cylinder_Test_Intersection};

    return scene;
}

#endif // __SINGLE_CYLINDER_H__