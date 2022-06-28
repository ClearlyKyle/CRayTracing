#include "Scene.h"

#include "SDL2/SDL.h"
#include "Image.h"
#include "Ray.h"
#include "Camera.h"
#include "Sphere.h"
#include "Light.h"

static struct Scene_s
{
    Image_t  output_image;
    Camera_t cam;
    Light_t  point_light;
} Scene;

void Scene_Init()
{
    SDL_Texture *texture = SDL_CreateTexture(app.renderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_STREAMING, app.width, app.height);

    // Initialise image
    Image_t image      = Image_Initialize(800, 800, app.renderer);
    Scene.output_image = image;

    Scene.cam = (Camera_t){
        .position        = {0.0, -10.0, 0.0},
        .lookat          = {0.0, 0.0, 0.0},
        .up              = {0.0, 0.0, 1.0},
        .length          = 1.0,
        .horizontal_size = 0.25,
        .aspect_ratio    = 1.0, // 800x800
    };

    Camera_Update_Geometry(&Scene.cam);

    Scene.point_light = (Light_t){.location = {5.0, -10.0, 5.0}, .colour = {255.0, 255.0, 255.0}, .intensity = 1.0};
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

            // Loop over each object in the sceene
            // for (Object *p = Scene.shapes, *end = &Scene.shapes[COUNT]; p != end; p++)
            // Call the correct "Test_Intersection" for a given shape..
            // Shape_Test_Intersection(shape.type, shape);

            // Test if we have a valid intersection.
            const bool validInt = Sphere_Test_Intersection(&cameraRay, &intPoint, &localNormal, localColor);

            // If we have a valid intersection, change pixel color to red.
            if (validInt)
            {
                // Compute intensity of illumination.
                double intensity;
                vec3   colour;
                bool   validIllum = false;

                // Loop over all lights...
                validIllum = Light_Compute_Illumination(&Scene.point_light, intPoint, localNormal, &colour, &intensity);

                // Compute the distance between the camera and the point of intersection.
                const double dist = vec3_length(vec3_sub(intPoint, cameraRay.point1));
                if (dist > maxDist)
                    maxDist = dist;

                if (dist < minDist)
                    minDist = dist;

                if (validIllum)
                {
                    Image_SetPixel(&Scene.output_image, x, y, 255.0 * intensity, 0.0, 0.0);
                }
                else
                {
                    Image_SetPixel(&Scene.output_image, x, y, 0.0, 0.0, 0.0);
                }
            }
            else
            {
                Image_SetPixel(&Scene.output_image, x, y, 0.0, 0.0, 0.0);
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
