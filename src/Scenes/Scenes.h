#ifndef __SCENES_H__
#define __SCENES_H__

#include "../Camera.h"

#include "../Materials/Material.h"
#include "../Lights/Lights.h"

typedef struct Scene
{
    Camera_t cam;

    size_t    mats_count;
    Material *mats;

    Objects objects;
    Lights  lights;
} Scene;

inline void Scene_Destroy(Scene *const scene)
{
    free(scene->mats);
    Objects_Free(&scene->objects);
    Lights_Free(&scene->lights);
}

#endif // __SCENES_H__