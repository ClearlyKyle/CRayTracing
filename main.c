#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdbool.h>
#include <time.h>

#include "Scene.h"

int main(int argc, char *argv[])
{
    App_Startup(800, 800, "Window Title something...",
                Scene_Init,
                Scene_Update,
                Scene_Exit);
    App_Start();

    return 0;
}