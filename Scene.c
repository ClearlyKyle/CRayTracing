#include "Scene.h"

#include "SDL2/SDL.h"
#include "Objects.h"
#include "Image.h"
#include "Ray.h"
#include "Camera.h"
#include "Sphere.h"
#include "Light.h"
#include "Plane.h"

#define NUMBER_OF_SPHERES 3
#define NUMBER_OF_LIGHTS 3
#define NUMBER_OF_PLANES 1

static struct Scene_s
{
    Image_t  output_image;
    Camera_t cam;

    ShapeArray shapes[4];
    size_t     num_of_shapes;

    Sphere  spheres[NUMBER_OF_SPHERES];
    Plane   planes[NUMBER_OF_PLANES];
    Light_t lights[NUMBER_OF_LIGHTS];
} Scene;

void Scene_Init()
{
    SDL_Texture *texture = SDL_CreateTexture(app.renderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_STREAMING, app.width, app.height);

    // Initialise image
    Image_t image      = Image_Initialize(800, 800, app.renderer);
    Scene.output_image = image;

    // Setup Camera
    Scene.cam = (Camera_t){
        .position        = {0.0, -10.0, 0.0},
        .lookat          = {0.0, 0.0, 0.0},
        .up              = {0.0, 0.0, 1.0},
        .length          = 1.0,
        .horizontal_size = 0.25,
        .aspect_ratio    = 1.0, // 800x800
    };
    Camera_Update_Geometry(&Scene.cam);

    // Setup Spheres
    Scene.spheres[0].colour = (vec3){0.25, 0.5, 0.8};
    Scene.spheres[1].colour = (vec3){1.0, 0.5, 0.0};
    Scene.spheres[2].colour = (vec3){1.0, 0.8, 0.0};

    Scene.spheres[0].transform = Transform_Set((vec3){-2.5, -1.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){0.5, 0.5, 0.75});
    Scene.spheres[1].transform = Transform_Set((vec3){0.0, 0.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){0.75, 0.5, 0.5});
    Scene.spheres[2].transform = Transform_Set((vec3){2.0, 0.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){0.75, 0.75, 0.75});

    // Setup Planes
    Scene.planes[0].colour    = (vec3){0.5, 0.5, 0.5};
    Scene.planes[0].transform = Transform_Set((vec3){0.0, 0.0, 0.75}, (vec3){0.0, 0.0, 0.0}, (vec3){4.0, 4.0, 1.0});

    // Scene Shapes
    Scene.shapes[0] = (ShapeArray){.type = SHAPE_SPHERE, .object.sphere = {.colour = {0.25, 0.5, 0.8}, .transform = Transform_Set((vec3){-2.5, -1.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){0.5, 0.5, 0.75})}};
    Scene.shapes[1] = (ShapeArray){.type = SHAPE_SPHERE, .object.sphere = {.colour = {1.0, 0.5, 0.0}, .transform = Transform_Set((vec3){0.0, 0.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){0.75, 0.5, 0.5})}};
    Scene.shapes[2] = (ShapeArray){.type = SHAPE_SPHERE, .object.sphere = {.colour = {1.0, 0.8, 0.0}, .transform = Transform_Set((vec3){2.0, 0.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){0.75, 0.75, 0.75})}};
    Scene.shapes[3] = (ShapeArray){.type = SHAPE_PLANE, .object.plane = {.colour = {0.5, 0.5, 0.5}, .transform = Transform_Set((vec3){0.0, 0.0, 0.75}, (vec3){0.0, 0.0, 0.0}, (vec3){4.0, 4.0, 1.0})}};

    // ShapeArray shapes[] = {
    //     {.type = SHAPE_SPHERE, .object.sphere = {.colour = {0.25, 0.5, 0.8}, .transform = Transform_Set((vec3){-2.5, -1.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){0.5, 0.5, 0.75})}},
    //     {.type = SHAPE_SPHERE, .object.sphere = {.colour = {1.0, 0.5, 0.0}, .transform = Transform_Set((vec3){0.0, 0.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){0.75, 0.5, 0.5})}},
    //     {.type = SHAPE_SPHERE, .object.sphere = {.colour = {1.0, 0.8, 0.0}, .transform = Transform_Set((vec3){2.0, 0.0, 0.0}, (vec3){0.0, 0.0, 0.0}, (vec3){0.75, 0.75, 0.75})}},
    //     {.type = SHAPE_PLANE, .object.plane = {.colour = {0.5, 0.5, 0.5}, .transform = Transform_Set((vec3){0.0, 0.0, 0.75}, (vec3){0.0, 0.0, 0.0}, (vec3){4.0, 4.0, 1.0})}},
    // };
    // Scene.shapes        = shapes;
    Scene.num_of_shapes = 4;

    // Setup Lights
    Scene.lights[0] = (Light_t){.location = {5.0, -10.0, -5.0}, .colour = {0.0, 0.0, 1.0}, .intensity = 1.0};
    Scene.lights[1] = (Light_t){.location = {-5.0, -10.0, -5.0}, .colour = {1.0, 0.0, 0.0}, .intensity = 1.0};
    Scene.lights[2] = (Light_t){.location = {0.0, -10.0, -5.0}, .colour = {0.0, 1.0, 0.0}, .intensity = 1.0};
}

void Scene_Update()
{
    const unsigned int x_size = Scene.output_image.x_size;
    const unsigned int y_size = Scene.output_image.y_size;

    // Loop over each pixel in our image.
    vec3 intPoint    = VEC3_INIT_ZERO;
    vec3 localNormal = VEC3_INIT_ZERO;
    vec3 localColor  = VEC3_INIT_ZERO;

    const double xFact   = 1.0 / ((double)(x_size) / 2.0);
    const double yFact   = 1.0 / ((double)(y_size) / 2.0);
    double       minDist = 1e6;
    double       maxDist = 0.0;

    for (unsigned int x = 0; x < x_size; x++)
    {
        for (unsigned int y = 0; y < y_size; y++)
        {
            // Normalize the x and y coordinates.
            double normX = ((double)x * xFact) - 1.0;
            double normY = ((double)y * yFact) - 1.0;

            // Generate the ray for this pixel.
            const Ray_t cameraRay = Camera_Generate_Ray(&Scene.cam, normX, normY);

            // Test for intersections with all objects in the scene.
            vec3 closestIntPoint    = VEC3_INIT_ZERO;
            vec3 closestLocalNormal = VEC3_INIT_ZERO;
            vec3 closestLocalColor  = VEC3_INIT_ZERO;

            double minDist            = 1e6;
            bool   intersection_found = false;

            size_t closestShape;

            // Loop over each object in the sceene
            // Call the correct "Test_Intersection" for a given shape..
            // Shape_Test_Intersection(shape.type, shape);
            // for (ShapeArray *shape = Scene.shapes, *end = &Scene.shapes[Scene.num_of_shapes]; shape != end; shape++)
            for (size_t i = 0; i < Scene.num_of_shapes; i++)
            {
                bool validInt = false;

                switch (Scene.shapes[i].type)
                {
                case SHAPE_SPHERE:
                    validInt = Sphere_Test_Intersection(Scene.shapes[i].object.sphere, &cameraRay, &intPoint, &localNormal, &localColor);
                    break;

                case SHAPE_PLANE:
                    validInt = Plane_Test_Intersection(Scene.shapes[i].object.plane, &cameraRay, &intPoint, &localNormal, &localColor);
                    break;

                default:
                    fprintf(stderr, "THIS SHAPE IS NOT SUPPORTED : %d\n", Scene.shapes[i].type);
                    app.running = false;
                    break;
                }

                if (validInt)
                {
                    intersection_found = true;

                    // Compute the distance between the camera and the point of intersection.
                    const double dist = vec3_length(vec3_sub(intPoint, cameraRay.point1));

                    if (dist < minDist)
                    {
                        closestShape       = i;
                        minDist            = dist;
                        closestIntPoint    = intPoint;
                        closestLocalNormal = localNormal;
                        closestLocalColor  = localColor;
                    }
                }
            }

            /* Compute the illumination for the closest object, assuming that there was a valid intersection. */
            if (intersection_found)
            {
                // Compute the intensity of illumination.
                double intensity;
                vec3   colour     = VEC3_INIT_ZERO;
                double red        = 0.0;
                double green      = 0.0;
                double blue       = 0.0;
                bool   illumFound = false;

                for (int i = 0; i < NUMBER_OF_LIGHTS; i++)
                {
                    const bool validIllum = Light_Compute_Illumination(&Scene.lights[i],
                                                                       closestShape,
                                                                       Scene.shapes,
                                                                       Scene.num_of_shapes,
                                                                       closestIntPoint,
                                                                       closestLocalNormal,
                                                                       &colour,
                                                                       &intensity);
                    // validIllum = currentLight->ComputeIllumination(closestIntPoint, closestLocalNormal, m_objectList, closestObject, color, intensity);
                    if (validIllum)
                    {
                        illumFound = true;
                        red += colour.x * intensity;
                        green += colour.y * intensity;
                        blue += colour.z * intensity;
                    }

                    if (illumFound)
                    {
                        red *= closestLocalColor.x;
                        green *= closestLocalColor.y;
                        blue *= closestLocalColor.z;

                        Image_SetPixel(&Scene.output_image, x, y, red, green, blue);
                    }
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
}
