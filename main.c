#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdbool.h>
#include <time.h>

#include "Window.h"

int main(int argc, char *argv[])
{
    Window_t window = SDL_Startup(800, 800, "Window Title something...");

    SDL_Texture *texture = SDL_CreateTexture(window.renderer, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_STREAMING, window.width, window.height);

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

        Uint32 *pixels = NULL;
        int     pitch  = 0;
        { // LOCK
            if (SDL_LockTexture(texture, NULL, &(void *)pixels, &pitch) < 0)
            {
                fprintf(stderr, "Couldn't lock texture: %s\n", SDL_GetError());

                running = false;
                break;
            }

            // clear to black background
            SDL_memset((void *)pixels, 0, pitch * window.height);

            // splat down some random pixels
            for (unsigned int i = 0; i < 1000; i++)
            {
                const unsigned int x = rand() % window.width;
                const unsigned int y = rand() % window.height;

                const Uint8 blue  = 000;              // b
                const Uint8 green = 255;              // g
                const Uint8 red   = 000;              // r
                const Uint8 alpha = SDL_ALPHA_OPAQUE; // a

                // Uint32 *pixel = (Uint32 *)((Uint8 *)pixels + y * pitch);
                const int index = (int)y * window.width + (int)x;

                pixels[index] = (Uint32)((alpha << 24) + (blue << 16) + (green << 8) + (red << 0)); // 0xAABBGGRR
            }
        }
        SDL_UnlockTexture(texture);

        // Copy to window
        SDL_RenderCopy(window.renderer, texture, NULL, NULL);
        SDL_RenderPresent(window.renderer);
    }

    return 0;
}