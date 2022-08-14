#include "Camera.h"

Camera_t Camera_Init()
{
    Camera_t cam = {
        .position        = {0.0, -10.0, 0.0},
        .lookat          = {0.0, 0.0, 0.0},
        .up              = {0.0, 0.0, 1.0},
        .length          = 1.0,
        .horizontal_size = 1.0,
        .aspect_ratio    = 1.0,
    };

    return cam;
}

void Camera_Update_Geometry(Camera_t *cam)
{
    // First, compute the vector from the camera position to the lookat position
    cam->alignment_vector = vec3_sub(cam->lookat, cam->position);
    NORMALISE_VEC3(cam->alignment_vector);

    // Second, compute the alpha and beta unit vectors
    cam->projection_screen_U = vec3_cross(cam->alignment_vector, cam->up);
    NORMALISE_VEC3(cam->projection_screen_U);
    cam->projection_screen_V = vec3_cross(cam->projection_screen_U, cam->alignment_vector);
    NORMALISE_VEC3(cam->projection_screen_V);

    // Thirdly, compute the position of the center point of the screen
    cam->projection_screen_center = vec3_add(cam->position, vec3_mul_scal(cam->alignment_vector, cam->length));

    // Modify the U and V vectors to match the size and aspect ration
    cam->projection_screen_U = vec3_mul_scal(cam->projection_screen_U, cam->horizontal_size);
    cam->projection_screen_V = vec3_mul_scal(cam->projection_screen_V, cam->horizontal_size / cam->aspect_ratio);
}

// This will return a Ray from a given Camera_t
Ray Camera_Generate_Ray(const Camera_t *cam, const double screen_x, const double screen_y)
{
    // Compute the location of the screen point in world coordinates
    const vec3 screenWorldPart1      = vec3_add(cam->projection_screen_center, vec3_mul_scal(cam->projection_screen_U, screen_x));
    const vec3 screenWorldCoordinate = vec3_add(screenWorldPart1, vec3_mul_scal(cam->projection_screen_V, screen_y));

    // Use this point along with the camera position to compute the ray.
    return Ray_Init(cam->position, screenWorldCoordinate);
}
