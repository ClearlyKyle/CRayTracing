#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdbool.h>
#include <time.h>

#include "App.h"
#include "Image.h"

int main(int argc, char *argv[])
{
    Application_t app = SDL_Startup(800, 800, "Window Title something...");

    SDL_Texture *texture = SDL_CreateTexture(app.renderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_STREAMING, app.width, app.height);

    const Uint64 PerfCountFrequency = SDL_GetPerformanceFrequency();
    Uint64       LastCounter        = SDL_GetPerformanceCounter();

    // Initialise image
    Image_t image = Image_Initialize(800, 800, app.renderer);

    // Create some colour variations
    for (int x = 0; x < 800; x++)
    {
        for (int y = 0; y < 800; y++)
        {
            const double red   = ((double)x / 800.0) * 255.0;
            const double green = ((double)y / 800.0) * 255.0;

            Image_SetPixel(&image, x, y, red, green, 0.0);
        }
    }

    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type)
            {
                running = false;
                break;
            }
        }

        // Background colour to white
        SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
        SDL_RenderClear(app.renderer);

        // On Render
        Image_Display(&image);

        // Show results...
        SDL_RenderPresent(app.renderer);

        // End frame timing
        Uint64 EndCounter     = SDL_GetPerformanceCounter();
        Uint64 CounterElapsed = EndCounter - LastCounter;

        const double MSPerFrame = (((1000.0f * (double)CounterElapsed) / (double)PerfCountFrequency));
        const double FPS        = (double)PerfCountFrequency / (double)CounterElapsed;

        fprintf(stdout, "%8.02f ms/f \t%8.02f f/s\n", MSPerFrame, FPS);

        LastCounter = EndCounter;
    }

    return 0;
}