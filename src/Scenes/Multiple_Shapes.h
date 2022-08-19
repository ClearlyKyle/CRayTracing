#ifndef __MULTIPLE_SHAPES_H__
#define __MULTIPLE_SHAPES_H__

#include "Scenes.h"

inline Scene Multiple_Shapes(void)
{
    Scene scene;

    // Setup Lights
    scene.lights.count  = 2;
    scene.lights.lights = (LightArray *)malloc(sizeof(LightArray) * scene.lights.count);

    scene.lights.lights[0] = (LightArray){.type = LIGHT_POINT, .object.pointLight = {.location = {3.0, -10.0, -5.0}, .colour = {1.0, 1.0, 1.0}, .intensity = 1.0}};
    scene.lights.lights[1] = (LightArray){.type = LIGHT_POINT, .object.pointLight = {.location = {0.0, -10.0, -5.0}, .colour = {1.0, 1.0, 1.0}, .intensity = 1.0}};

    // Setup Camera
    scene.cam = (Camera_t){
        .position        = {3.0, -5.0, -2.0},
        .lookat          = {0.0, 0.0, 0.0},
        .up              = {0.0, 0.0, 1.0},
        .length          = 1.0,
        .horizontal_size = 0.75,
        .aspect_ratio    = 1.0, // 800x800
    };
    Camera_Update_Geometry(&scene.cam);

    // Materials
    scene.mats_count = 7;
    scene.mats       = (Material *)malloc(sizeof(Material) * scene.mats_count);
    scene.mats[0]    = (Material){.base_colour = {0.5, 0.5, 0.8}, .reflectivity = 0.5, .shininess = 20.0};     // Silver Metal
    scene.mats[1]    = (Material){.base_colour = {0.8, 0.8, 0.3}, .reflectivity = 0.25, .shininess = 20.0};    // Gold Metal
    scene.mats[2]    = (Material){.base_colour = {0.2, 0.2, 0.8}, .reflectivity = 0.05, .shininess = 5.0};     // Blue Diffuse
    scene.mats[3]    = (Material){.base_colour = {0.8, 0.8, 0.2}, .reflectivity = 0.05, .shininess = 5.0};     // Yellow Diffuse
    scene.mats[4]    = (Material){.base_colour = {1.0, 0.5, 0.0}, .reflectivity = 0.05, .shininess = 5.0};     // Orange Diffuse
    scene.mats[5]    = (Material){.base_colour = {1.0, 1.0, 1.0}, .reflectivity = 0.0, .shininess = 0.0};      // Floor
    scene.mats[6]    = (Material){.base_colour = {1.0, 0.125, 0.125}, .reflectivity = 0.75, .shininess = 0.0}; // Wall

    // Setup Objects (Spheres, Planes)
    scene.objects = Objects_Init(10);

    scene.objects.shapes[0] = (Shape){.type              = SHAPE_CONE,
                                      .mat               = &scene.mats[0], // Silver Metal
                                      .transform         = Transform_Set((vec3){0.0, 0.0, -0.5}, (vec3){0.0, 0.0, 0.0}, (vec3){1.0, 1.0, 2.0})};

    scene.objects.shapes[1] = (Shape){.type              = SHAPE_SPHERE,   // Left Sphere
                                      .mat               = &scene.mats[2], // Blue Diffuse
                                      .transform         = Transform_Set((vec3){1.5, -2.0, 0.5}, (vec3){0.0, 0.0, 0.0}, (vec3){0.5, 0.5, 0.5})};

    scene.objects.shapes[2] = (Shape){.type              = SHAPE_SPHERE,   // Right Sphere
                                      .mat               = &scene.mats[3], // Yellow Diffuse
                                      .transform         = Transform_Set((vec3){1.5, 0.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){1.0, 1.0, 1.0})};

    scene.objects.shapes[3] = (Shape){.type              = SHAPE_SPHERE,   // Top Sphere
                                      .mat               = &scene.mats[4], // Orange Diffuse
                                      .transform         = Transform_Set((vec3){0.0, 0.0, -1.0}, (vec3){0.0, 0.0, 0.0}, (vec3){0.5, 0.5, 0.5})};

    scene.objects.shapes[4] = (Shape){.type              = SHAPE_PLANE,    // Floor
                                      .mat               = &scene.mats[5], // Floor
                                      .transform         = Transform_Set((vec3){0.0, 0.0, 1.0}, (vec3){0.0, 0.0, 0.0}, (vec3){16.0, 16.0, 1.0})};

    scene.objects.shapes[5] = (Shape){.type              = SHAPE_PLANE,    // Left Wall
                                      .mat               = &scene.mats[6], // Wall
                                      .transform         = Transform_Set((vec3){-4.0, 0.0, 0.0}, (vec3){0.0, -M_PI / 2.0, -M_PI / 2.0}, (vec3){16.0, 16.0, 1.0})};

    scene.objects.shapes[6] = (Shape){.type              = SHAPE_PLANE, // Right Wall
                                      .mat               = &scene.mats[6],
                                      .transform         = Transform_Set((vec3){0.0, 4.0, 0.0}, (vec3){-M_PI / 2.0, 0.0, 0.0}, (vec3){16.0, 16.0, 1.0})};

    scene.objects.shapes[7] = (Shape){.type              = SHAPE_CYLINDER, // Cylinder1
                                      .mat               = &scene.mats[1], // Gold Metal
                                      .transform         = Transform_Set((vec3){-1.5, -2.0, 1.0}, (vec3){0.0, -M_PI / 2.0, 0.0}, (vec3){0.25, 0.25, 1.0})};

    scene.objects.shapes[8] = (Shape){.type              = SHAPE_CYLINDER, // Cylinder2
                                      .mat               = &scene.mats[0], // Silver Metal
                                      .transform         = Transform_Set((vec3){-1.0, -2.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){0.25, 0.25, 1.0})};

    scene.objects.shapes[9] = (Shape){.type              = SHAPE_CONE,     // Cone2
                                      .mat               = &scene.mats[1], // Gold Metal
                                      .transform         = Transform_Set((vec3){0.0, -1.0, 0.0}, (vec3){M_PI / 4.0, 0.0, 0.0}, (vec3){0.5, 0.5, 1.0})};

    return scene;
}
#endif // __MULTIPLE_SHAPES_H__