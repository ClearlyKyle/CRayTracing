#include "Ray_Tracing.h"

#include "SDL2/SDL.h"

#include "Camera.h"
#include "Image.h"

#include "Scenes/Single_Cylinder.h"
#include "Scenes/Multiple_Shapes.h"

static struct Raycaster
{
    Image_t output_image;

    Scene scene;

} Raycaster;

void Ray_Tracing_Init()
{
    // Initialise image
    Image_t image          = Image_Initialize(app.width, app.height, app.renderer);
    Raycaster.output_image = image;

    // Raycaster.scene = Single_Cylinder();
    Raycaster.scene = Multiple_Shapes();
}

void Ray_Tracing_Update()
{
    const unsigned int x_size = Raycaster.output_image.x_size;
    const unsigned int y_size = Raycaster.output_image.y_size;

    const double xFact = 1.0 / ((double)(x_size) / 2.0);
    const double yFact = 1.0 / ((double)(y_size) / 2.0);

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
            const Ray cameraRay = Camera_Generate_Ray(&Raycaster.scene.cam, normX, normY);

            // Test for intersections with all objects in the scene.
            size_t closest_object_index = 0;

            // CAST RAY
            vec3   intPoint           = VEC3_INIT_ZERO;
            vec3   localNormal        = VEC3_INIT_ZERO;
            vec3   localColor         = VEC3_INIT_ZERO;
            double minDist            = 1e6;
            bool   intersection_found = false;

            // Loop over each object in the sceene
            for (size_t object_index = 0; object_index < Raycaster.scene.objects.count; object_index++)
            {
                const bool valid_intersection = Object_Test_Intersection(Raycaster.scene.objects.shapes[object_index],
                                                                         cameraRay,
                                                                         &intPoint,
                                                                         &localNormal,
                                                                         &localColor);
                if (valid_intersection)
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
                if (Raycaster.scene.objects.shapes[closest_object_index].mat)
                {
                    // Use the material to compute the color.
                    Material_Set_Reflection_Ray_Count(0);

                    // Call the matrials Compute_Colour - eg, basic material
                    const vec3 colour = Simple_Material_Compute_Colour(*Raycaster.scene.objects.shapes[closest_object_index].mat,
                                                                       Raycaster.scene.objects,
                                                                       Raycaster.scene.lights,
                                                                       closest_object_index,
                                                                       &closestIntPoint,
                                                                       &closestLocalNormal,
                                                                       &cameraRay);

                    Image_SetPixel(&Raycaster.output_image, x, y, colour.x, colour.y, colour.z);
                }
                else
                {
                    // Use the basic method to compute the color.
                    const vec3 colour = Material_Compute_Diffuse_Colour(Raycaster.scene.objects,
                                                                        Raycaster.scene.lights,
                                                                        closest_object_index,
                                                                        &closestIntPoint,
                                                                        &closestLocalNormal,
                                                                        Raycaster.scene.objects.shapes[closest_object_index].mat->base_colour);

                    Image_SetPixel(&Raycaster.output_image, x, y, colour.x, colour.y, colour.z);
                }
            }
        }
    }
    Image_Display(&Raycaster.output_image);
}

void Ray_Tracing_Exit()
{
    Image_Free(&Raycaster.output_image);

    Scene_Destroy(&Raycaster.scene);
}
