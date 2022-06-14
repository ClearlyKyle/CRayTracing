#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "SDL2/SDL.h"

typedef struct Application_s
{
    SDL_Window   *window;
    SDL_Renderer *renderer;

    unsigned int width;
    unsigned int height;

    double frame_time, last_frame_time; // ms
} Application_t;

Application_t SDL_Startup(const int width, const int height, const char *title);
void          SDL_Exit(Application_t *window);

#endif // __WINDOW_H__