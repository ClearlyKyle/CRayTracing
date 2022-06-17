#include "Scene.h"

void Scene_Render(Image_t *output_image)
{
    const unsigned int x_size = output_image->x_size;
    const unsigned int y_size = output_image->y_size;

    for (unsigned int x = 0; x < x_size; x++)
    {
        for (unsigned int y = 0; y < y_size; y++)
        {
            const double r = ((double)x / (double)x_size) * 255.0;
            const double g = ((double)y / (double)y_size) * 255.0;

            Image_SetPixel(output_image, x, y, r, g, 0.0);
        }
    }
}
