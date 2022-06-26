#include "App.h"

Application_t app;

static void _Init()
{
    assert(app.window != NULL);

    app.Init();
}

static void _Update()
{
    app.Update();
}

static void _OnExit()
{
    app.OnExit();

    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
}

static void _PollEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (SDL_QUIT == event.type)
        {
            app.running = false;
            break;
        }
    }
}

void App_Startup(const int width, const int height, const char *title,
                 Appfunction_ptr init,    // run on application startup
                 Appfunction_ptr update,  // run on every loop
                 Appfunction_ptr on_exit) // run on exit of application
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL cannot init with error: %s\n", SDL_GetError());
        exit(1);
    }

    app.window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_SHOWN); // show upon creation

    if (app.window == NULL)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());

        SDL_Quit();

        exit(1);
    }

    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    if (app.renderer == NULL)
    {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());

        SDL_DestroyWindow(app.window);
        SDL_Quit();

        exit(1);
    }

    app.height = height;
    app.width  = width;

    app.Init   = init;
    app.Update = update;
    app.OnExit = on_exit;
}

void App_Start()
{
    _Init();

    float elapsed = 0.0f;

    const Uint64 PerfCountFrequency = SDL_GetPerformanceFrequency();
    Uint64       LastCounter        = SDL_GetPerformanceCounter();

    app.running = true;

    while (app.running)
    {
        Uint64 start        = SDL_GetPerformanceCounter();
        app.last_frame_time = app.frame_time;
        app.frame_time      = elapsed;

        // Background colour to white
        SDL_SetRenderDrawColor(app.renderer, 176, 196, 222, 255);
        SDL_RenderClear(app.renderer);

        _PollEvents();
        _Update();

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

    _OnExit();
}
