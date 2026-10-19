#include "simulation.h"
#include <raylib.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

Simulation simulation;

void UpdateDrawFrame(void);

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
#ifndef PLATFORM_WEB
    ToggleBorderlessWindowed();
    SetTargetFPS(0);
#endif
    InitWindow(1280, 720, "gas");
    InitAudioDevice();
    
    simulation.init();
#if defined(PLATFORM_WEB)
    SetTargetFPS(144);
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }
#endif
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void UpdateDrawFrame() {
    BeginDrawing();
    ClearBackground(BLACK);
    
    simulation.tick();
    simulation.draw();

    EndDrawing();
}
