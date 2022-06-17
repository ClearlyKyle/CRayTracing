#ifndef __SCENE_H__
#define __SCENE_H__

#include "SDL2/SDL.h"
#include "Image.h"

struct Scene_s
{
    int something;
};

typedef struct Scene_s Scene_t;

void Scene_Render(Image_t *output_image);

#endif // __SCENE_H__