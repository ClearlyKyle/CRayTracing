#include "Window.h"

Window_t SDL_Startup(const int width, const int height, const char *title)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL cannot init with error: %s\n", SDL_GetError());
        exit(1);
    }

    Window_t window;
    window.window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_SHOWN); // show upon creation

    if (window.window == NULL)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());

        SDL_Quit();

        exit(1);
    }

    window.renderer = SDL_CreateRenderer(window.window, -1, SDL_RENDERER_ACCELERATED);
    if (window.renderer == NULL)
    {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());

        SDL_DestroyWindow(window.window);
        SDL_Quit();

        exit(1);
    }

    window.height = height;
    window.width  = width;

    return window;
}

void SDL_Exit(Window_t *window)
{
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    SDL_Quit();
}