#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "SDL2/SDL.h"

typedef struct Window_s
{
    SDL_Window   *window;
    SDL_Renderer *renderer;

    unsigned int width;
    unsigned int height;

    double frame_time, last_frame_time; // ms
} Window_t;

Window_t SDL_Startup(const int width, const int height, const char *title);
void     SDL_Exit(Window_t *window);

#endif // __WINDOW_H__