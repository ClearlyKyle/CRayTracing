#ifndef __SCENE_CUBE_H__
#define __SCENE_CUBE_H__

#include "Scenes.h"

inline Scene Scene_Cube(void)
{
    Scene scene;

    // Setup Lights
    scene.lights.count  = 2;
    scene.lights.lights = (LightArray *)malloc(sizeof(LightArray) * scene.lights.count);

    scene.lights.lights[0] = (LightArray){.type = LIGHT_POINT, .object.pointLight = {.location = {3.0, -10.0, -5.0}, .colour = {1.0, 1.0, 1.0}, .intensity = 1.0}};
    scene.lights.lights[1] = (LightArray){.type = LIGHT_POINT, .object.pointLight = {.location = {0.0, -10.0, -5.0}, .colour = {1.0, 1.0, 1.0}, .intensity = 1.0}};

    // Setup Camera
    scene.cam = (Camera_t){
        .position        = {2.0, -5.0, -1.0},
        .lookat          = {0.0, 0.0, 0.0},
        .up              = {0.0, 0.0, 1.0},
        .length          = 1.0,
        .horizontal_size = 1.0,
        .aspect_ratio    = 1.0, // 800x800
    };
    Camera_Update_Geometry(&scene.cam);

    // Textures
    Texture floor_texture = Texture_Checker_Init((vec2){0.0, 0.0}, 0.0, (vec2){16.0, 16.0}, (vec4){0.0, 0.0, 0.0, 1.0}, (vec4){0.2, 0.2, 0.2, 1.0});
    Texture image_texture = Texture_Image_Init("../../res/images/crate_texture.bmp", (vec2){0.0, 0.0}, 0.0, (vec2){1.0, 1.0});

    // Materials
    scene.mats_count      = 6;
    scene.mats            = (Material *)malloc(sizeof(Material) * scene.mats_count);
    scene.mats[0]         = (Material){.type = MATERIAL_SIMPLE, .base_colour = {0.0, 1.0, 0.0}, .reflectivity = 0.25, .shininess = 0.0, .has_texture = true}; // Floor
    scene.mats[0].texture = floor_texture;
    scene.mats[1]         = (Material){.type = MATERIAL_SIMPLE, .base_colour = {0.0, 0.0, 1.0}, .reflectivity = 0.0, .shininess = 0.0, .has_texture = true}; // Image Material
    scene.mats[1].texture = image_texture;
    scene.mats[2]         = (Material){.type = MATERIAL_SIMPLE, .base_colour = {1.0, 0.2, 0.2}, .reflectivity = 0.8, .shininess = 32.0, .has_texture = false};  // Sphere Material 1
    scene.mats[3]         = (Material){.type = MATERIAL_SIMPLE, .base_colour = {0.2, 1.0, 0.2}, .reflectivity = 0.8, .shininess = 32.0, .has_texture = false};  // Sphere Material 2
    scene.mats[4]         = (Material){.type = MATERIAL_SIMPLE, .base_colour = {0.2, 0.2, 1.0}, .reflectivity = 0.8, .shininess = 32.0, .has_texture = false};  // Sphere Material 3
    scene.mats[5]         = (Material){.type = MATERIAL_SIMPLE, .base_colour = {0.7, 0.7, 0.2}, .reflectivity = 0.15, .shininess = 32.0, .has_texture = false}; // Cube Material

    // Setup Objects (Spheres, Planes)
    scene.objects = Objects_Init(6);

    scene.objects.shapes[0] = (Shape){.type      = SHAPE_PLANE,    // Floor
                                      .mat       = &scene.mats[0], // Floor
                                      .visible   = true,
                                      .transform = Transform_Set((vec3){0.0, 0.0, 1.0}, (vec3){0.0, 0.0, 0.0}, (vec3){16.0, 16.0, 16.0}, true)};

    scene.objects.shapes[1] = (Shape){.type      = SHAPE_PLANE,    // Image Plane
                                      .mat       = &scene.mats[1], // Image Material
                                      .visible   = true,
                                      .transform = Transform_Set((vec3){0.0, 5.0, -0.75}, (vec3){-M_PI / 2.0, 0.0, 0.0}, (vec3){1.75, 1.75, 1.0}, true)};

    scene.objects.shapes[2] = (Shape){.type      = SHAPE_SPHERE,   // Sphere
                                      .mat       = &scene.mats[2], // Sphere Material 1
                                      .visible   = true,
                                      .transform = Transform_Set((vec3){-2.0, -2.0, 0.25}, (vec3){0.0, 0.0, 0.0}, (vec3){0.75, 0.75, 0.75}, true)};

    scene.objects.shapes[3] = (Shape){.type      = SHAPE_SPHERE,   // Sphere
                                      .mat       = &scene.mats[3], // Sphere Material 2
                                      .visible   = true,
                                      .transform = Transform_Set((vec3){-2.0, -0.5, 0.25}, (vec3){0.0, 0.0, 0.0}, (vec3){0.75, 0.75, 0.75}, true)};

    scene.objects.shapes[4] = (Shape){.type      = SHAPE_SPHERE,   // Sphere
                                      .mat       = &scene.mats[4], // Sphere Material 3
                                      .visible   = true,
                                      .transform = Transform_Set((vec3){-2.0, -1.25, -1.0}, (vec3){0.0, 0.0, 0.0}, (vec3){0.75, 0.75, 0.75}, true)};

    scene.objects.shapes[5] = (Shape){.type      = SHAPE_CUBE,     // Cube
                                      .mat       = &scene.mats[5], // Cube Material
                                      .visible   = true,
                                      .transform = Transform_Set((vec3){2.0, -1.25, 0.25}, (vec3){0.0, 0.0, 0.0}, (vec3){0.75, 1.0, 0.75}, true)};

    return scene;
}

#endif // __SCENE_CUBE_H__