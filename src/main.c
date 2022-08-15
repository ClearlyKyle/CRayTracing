#include "Ray_Tracing.h"

int main(int argc, char *argv[])
{
    argc = 0;
    argv = NULL;

    App_Startup(800, 800, "Window Title something...",
                Ray_Tracing_Init,
                Ray_Tracing_Update,
                Ray_Tracing_Exit);
    App_Start();

    return 0;
}