#include "Scene.h"

#include "SDL2/SDL.h"

#include "Camera.h"
#include "Image.h"

#include "Material.h"

static struct Scene_s
{
    Image_t  output_image;
    Camera_t cam;

    Material mats[4];

    Objects objects;
    Lights  lights;
} Scene;

void Scene_Init()
{
    SDL_Texture *texture = SDL_CreateTexture(app.renderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_STREAMING, app.width, app.height);

    // Initialise image
    Image_t image      = Image_Initialize(app.width, app.height, app.renderer);
    Scene.output_image = image;

    // Setup Camera
    Scene.cam = (Camera_t){
        .position        = {0.0, -10.0, -2.0},
        .lookat          = {0.0, 0.0, 0.0},
        .up              = {0.0, 0.0, 1.0},
        .length          = 1.0,
        .horizontal_size = 0.25,
        .aspect_ratio    = 1.0, // 800x800
    };
    Camera_Update_Geometry(&Scene.cam);

    // Materials
    Scene.mats[0] = (Material){.base_colour = {0.25, 0.5, 0.8}, .reflectivity = 0.1, .shininess = 10.0};
    Scene.mats[1] = (Material){.base_colour = {1.0, 0.5, 0.0}, .reflectivity = 0.75, .shininess = 10.0};
    Scene.mats[2] = (Material){.base_colour = {1.0, 0.8, 0.0}, .reflectivity = 0.25, .shininess = 10.0};
    Scene.mats[3] = (Material){.base_colour = {1.0, 1.0, 1.0}, .reflectivity = 0.5, .shininess = 0.0};

    // Setup Objects (Spheres, Planes)
    Scene.objects = Objects_Init(4);

    Scene.objects.shapes[0] = (Shape){.type              = SHAPE_SPHERE,
                                      .mat               = &Scene.mats[2],
                                      .base_colour       = {0.25, 0.5, 0.8},
                                      .transform         = Transform_Set((vec3){-1.5, 0.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){0.5, 0.5, 0.5}),
                                      .Test_Intersection = &Sphere_Test_Intersection};

    Scene.objects.shapes[1] = (Shape){.type              = SHAPE_CONE,
                                      .mat               = &Scene.mats[0],
                                      .base_colour       = {1.0, 0.5, 0.0},
                                      .transform         = Transform_Set((vec3){0.0, 0.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){0.5, 0.5, 0.5}),
                                      .Test_Intersection = &Cone_Test_Intersection};

    Scene.objects.shapes[2] = (Shape){.type              = SHAPE_CYLINDER,
                                      .mat               = &Scene.mats[1],
                                      .base_colour       = {1.0, 0.8, 0.0},
                                      .transform         = Transform_Set((vec3){1.5, 0.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){0.5, 0.5, 0.5}),
                                      .Test_Intersection = &Cylinder_Test_Intersection};

    Scene.objects.shapes[3] = (Shape){.type              = SHAPE_PLANE,
                                      .mat               = &Scene.mats[3],
                                      .base_colour       = {0.5, 0.5, 0.5},
                                      .transform         = Transform_Set((vec3){0.0, 0.0, 0.75}, (vec3){0.0, 0.0, 0.0}, (vec3){4.0, 4.0, 1.0}),
                                      .Test_Intersection = &Plane_Test_Intersection};

    // Setup Lights
    Scene.lights.count  = 3;
    Scene.lights.lights = (LightArray *)malloc(sizeof(LightArray) * Scene.lights.count);

    Scene.lights.lights[0] = (LightArray){.type = LIGHT_POINT, .object.pointLight = {.location = {5.0, -10.0, -5.0}, .colour = {0.0, 0.0, 1.0}, .intensity = 1.0}};
    Scene.lights.lights[1] = (LightArray){.type = LIGHT_POINT, .object.pointLight = {.location = {-5.0, -10.0, -5.0}, .colour = {1.0, 0.0, 0.0}, .intensity = 1.0}};
    Scene.lights.lights[2] = (LightArray){.type = LIGHT_POINT, .object.pointLight = {.location = {0.0, -10.0, -5.0}, .colour = {0.0, 1.0, 0.0}, .intensity = 1.0}};
}

void Scene_Update()
{
    const unsigned int x_size = Scene.output_image.x_size;
    const unsigned int y_size = Scene.output_image.y_size;

    const double xFact   = 1.0 / ((double)(x_size) / 2.0);
    const double yFact   = 1.0 / ((double)(y_size) / 2.0);
    double       minDist = 1e6;
    double       maxDist = 0.0;

    vec3 intPoint    = VEC3_INIT_ZERO;
    vec3 localNormal = VEC3_INIT_ZERO;
    vec3 localColor  = VEC3_INIT_ZERO;
    vec3 base_colour = VEC3_INIT_ZERO;

    vec3 closestIntPoint    = VEC3_INIT_ZERO;
    vec3 closestLocalNormal = VEC3_INIT_ZERO;
    vec3 closestLocalColor  = VEC3_INIT_ZERO;

    for (unsigned int x = 0; x < x_size; x++)
    {
        for (unsigned int y = 0; y < y_size; y++)
        {
            // Normalize the x and y coordinates.
            const double normX = ((double)x * xFact) - 1.0;
            const double normY = ((double)y * yFact) - 1.0;

            // Generate the ray for this pixel.
            const Ray_t cameraRay = Camera_Generate_Ray(&Scene.cam, normX, normY);

            // Test for intersections with all objects in the scene.
            size_t closest_object_index = 0;

            // CAST RAY
            // Loop over each pixel in our image.
            double    minDist            = 1e6;
            bool      intersection_found = false;
            Material *material           = NULL;

            // Loop over each object in the sceene
            for (size_t object_index = 0; object_index < Scene.objects.count; object_index++)
            {
                const bool validInt = Scene.objects.shapes[object_index].Test_Intersection(Scene.objects.shapes[object_index].transform,
                                                                                           Scene.objects.shapes[object_index].base_colour,
                                                                                           cameraRay,
                                                                                           &intPoint,
                                                                                           &localNormal,
                                                                                           &localColor);

                if (validInt)
                {
                    intersection_found = true;

                    // Compute the distance between the camera and the point of intersection.
                    const double dist = vec3_length(vec3_sub(intPoint, cameraRay.point1));

                    if (dist < minDist)
                    {
                        minDist              = dist;
                        closest_object_index = object_index;
                        closestIntPoint      = intPoint;
                        closestLocalNormal   = localNormal;
                        closestLocalColor    = localColor;
                    }
                }
            }

            /* Compute the illumination for the closest object, assuming that there was a valid intersection. */
            if (intersection_found)
            {
                // Check if the object has a material.
                if (Scene.objects.shapes[closest_object_index].mat)
                {
                    // Use the material to compute the color.
                    Material_Set_Reflection_Ray_Count(0);

                    // Call the matrials Compute_Colour - eg, basic material
                    const vec3 colour = Simple_Material_Compute_Colour(*Scene.objects.shapes[closest_object_index].mat, Scene.objects, Scene.lights, closest_object_index, &closestIntPoint, &closestLocalNormal, &cameraRay);

                    Image_SetPixel(&Scene.output_image, x, y, colour.x, colour.y, colour.z);
                }
                else
                {
                    // Use the basic method to compute the color.
                    const vec3 colour = Material_Compute_Diffuse_Colour(Scene.objects, Scene.lights, closest_object_index, &closestIntPoint, &closestLocalNormal, base_colour);

                    Image_SetPixel(&Scene.output_image, x, y, colour.x, colour.y, colour.z);
                }
            }
        }
    }

    printf("Minimum distance: %f\n", minDist);
    printf("Maximum distance: %f\n", maxDist);

    Image_Display(&Scene.output_image);
}

void Scene_Exit()
{
    Image_Free(&Scene.output_image);

    Objects_Free(&Scene.objects);
}
