#ifndef __SCENE_TEXTURE_MAPPING_H__
#define __SCENE_TEXTURE_MAPPING_H__

#include "Scenes.h"

#include "../Textures/Textures.h"

inline Scene Scene_Texture_Mapping(void)
{
    Scene scene;

    // Setup Lights
    scene.lights.count  = 2;
    scene.lights.lights = (LightArray *)malloc(sizeof(LightArray) * scene.lights.count);

    scene.lights.lights[0] = (LightArray){.type = LIGHT_POINT, .object.pointLight = {.location = {3.0, -10.0, -5.0}, .colour = {1.0, 1.0, 1.0}, .intensity = 1.0}};
    scene.lights.lights[1] = (LightArray){.type = LIGHT_POINT, .object.pointLight = {.location = {0.0, -10.0, -5.0}, .colour = {1.0, 1.0, 1.0}, .intensity = 1.0}};

    // Setup Camera
    scene.cam = (Camera_t){
        .position        = {2.0, -5.0, -2.0},
        .lookat          = {0.0, 0.0, 0.0},
        .up              = {0.0, 0.0, 1.0},
        .length          = 1.0,
        .horizontal_size = 1.0,
        .aspect_ratio    = 1.0, // 800x800
    };
    Camera_Update_Geometry(&scene.cam);

    // Textures
    Texture floor_texture    = Texture_Checker_Init((vec2){0.0, 0.0}, 0.0, (vec2){16.0, 16.0}, (vec4){0.0, 0.0, 0.0, 1.0}, (vec4){0.2, 0.2, 0.2, 1.0});
    Texture sphere_texture   = Texture_Checker_Init((vec2){0.0, 0.0}, 0.0, (vec2){16.0, 16.0}, (vec4){0.2, 0.2, 0.8, 1.0}, (vec4){0.8, 0.8, 0.2, 1.0});
    Texture cylinder_texture = Texture_Checker_Init((vec2){0.0, 0.0}, 0.0, (vec2){4.0 * M_PI, 4.0}, (vec4){1.0, 0.5, 0.0, 1.0}, (vec4){0.8, 0.8, 0.2, 1.0});
    Texture cone_texture     = Texture_Checker_Init((vec2){0.0, 0.0}, 0.0, (vec2){8.0 * (M_PI / 2.0), 8.0}, (vec4){0.2, 0.2, 0.8, 1.0}, (vec4){1.0, 0.5, 0.0, 1.0});

    // Materials
    scene.mats_count      = 7;
    scene.mats            = (Material *)malloc(sizeof(Material) * scene.mats_count);
    scene.mats[0]         = (Material){.type = MATERIAL_SIMPLE, .base_colour = {0.5, 0.5, 0.8}, .reflectivity = 0.5, .shininess = 20.0};                      // Silver Metal
    scene.mats[1]         = (Material){.type = MATERIAL_SIMPLE, .base_colour = {0.8, 0.8, 0.3}, .reflectivity = 0.25, .shininess = 20.0};                     // Gold Metal
    scene.mats[2]         = (Material){.type = MATERIAL_SIMPLE, .base_colour = {0.0, 0.0, 0.0}, .reflectivity = 0.05, .shininess = 5.0, .has_texture = true}; // Blue Diffuse
    scene.mats[2].texture = cone_texture;
    scene.mats[3]         = (Material){.type = MATERIAL_SIMPLE, .base_colour = {1.0, 0.0, 0.0}, .reflectivity = 0.05, .shininess = 5.0, .has_texture = true}; // Yellow Diffuse
    scene.mats[3].texture = sphere_texture;
    scene.mats[4]         = (Material){.type = MATERIAL_SIMPLE, .base_colour = {1.0, 0.0, 0.0}, .reflectivity = 0.05, .shininess = 5.0, .has_texture = true}; // Orange Diffuse
    scene.mats[4].texture = cylinder_texture;
    scene.mats[5]         = (Material){.type = MATERIAL_SIMPLE, .base_colour = {0.0, 1.0, 0.0}, .reflectivity = 0.5, .shininess = 0.0, .has_texture = true}; // Floor
    scene.mats[5].texture = floor_texture;
    scene.mats[6]         = (Material){.type = MATERIAL_SIMPLE, .base_colour = {1.0, 0.125, 0.125}, .reflectivity = 0.75, .shininess = 0.0}; // Wall

    // Setup Objects (Spheres, Planes)
    scene.objects = Objects_Init(7);

    scene.objects.shapes[0] = (Shape){.type      = SHAPE_CONE,     // Cone
                                      .mat       = &scene.mats[2], // Blue Diffuse
                                      .visible   = true,
                                      .transform = Transform_Set((vec3){-1.0, -2.0, -2.0}, (vec3){0.0, 0.0, 0.0}, (vec3){0.5, 0.5, 1.0}, false)};

    scene.objects.shapes[1] = (Shape){.type      = SHAPE_SPHERE,   // Left Sphere
                                      .mat       = &scene.mats[0], // Silver Metal
                                      .visible   = true,
                                      .transform = Transform_Set((vec3){1.0, -1.0, 0.5}, (vec3){0.0, 0.0, 0.0}, (vec3){0.5, 0.5, 0.5}, false)};

    scene.objects.shapes[2] = (Shape){.type      = SHAPE_SPHERE,   // Right Sphere
                                      .mat       = &scene.mats[3], // Yellow Diffuse
                                      .visible   = true,
                                      .transform = Transform_Set((vec3){2.0, 0.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){1.0, 1.0, 1.0}, false)};

    scene.objects.shapes[3] = (Shape){.type      = SHAPE_PLANE,    // Floor
                                      .mat       = &scene.mats[5], // Floor
                                      .visible   = true,
                                      .transform = Transform_Set((vec3){0.0, 0.0, 1.0}, (vec3){0.0, 0.0, 0.0}, (vec3){16.0, 16.0, 1.0}, false)};

    scene.objects.shapes[4] = (Shape){.type      = SHAPE_PLANE,    // Left Wall
                                      .mat       = &scene.mats[6], // Wall
                                      .visible   = true,
                                      .transform = Transform_Set((vec3){-4.0, 0.0, 0.0}, (vec3){0.0, -M_PI / 2.0, -M_PI / 2.0}, (vec3){16.0, 16.0, 1.0}, false)};

    scene.objects.shapes[5] = (Shape){.type      = SHAPE_PLANE,    // Right Wall
                                      .mat       = &scene.mats[6], // Wall
                                      .visible   = true,
                                      .transform = Transform_Set((vec3){0.0, 4.0, 0.0}, (vec3){-M_PI / 2.0, 0.0, 0.0}, (vec3){16.0, 16.0, 1.0}, false)};

    scene.objects.shapes[6] = (Shape){.type      = SHAPE_CYLINDER, // Cylinder1
                                      .mat       = &scene.mats[1], // Orange Diffuse
                                      .visible   = true,
                                      .transform = Transform_Set((vec3){-1.0, -2.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){1.0, 1.0, 1.0}, false)};
    return scene;
}

#endif // __SCENE_TEXTURE_MAPPING_H__