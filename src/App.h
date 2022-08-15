#ifndef __APP_H__
#define __APP_H__

#include <assert.h>
#include <stdbool.h>

#include "SDL2/SDL.h"
#include "Ray_Tracing.h"

typedef void (*Appfunction_ptr)();

typedef struct Application_s
{
    SDL_Window   *window;
    SDL_Renderer *renderer;

    unsigned int width;
    unsigned int height;

    bool running;

    Appfunction_ptr Init;
    Appfunction_ptr Update;
    Appfunction_ptr OnExit;

    double frame_time, last_frame_time; // ms
} Application_t;

// global app
extern struct Application_s app;

void App_Startup(const int width, const int height, const char *title,
                 Appfunction_ptr init,     // run on application startup
                 Appfunction_ptr update,   // run on every loop
                 Appfunction_ptr on_exit); // run on exit of application
void App_Start();

#endif // __APP_H__