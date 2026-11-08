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
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
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
#if defined(PLATFORM_WEB)
    if (IsWindowResized())
    {
        double dpr = emscripten_get_device_pixel_ratio();

        emscripten_set_canvas_size(
            GetScreenWidth() * dpr,
            GetScreenHeight() * dpr
        );
    }
#endif
    BeginDrawing();
    ClearBackground(BLACK);
    
    simulation.tick();
    simulation.draw();

    EndDrawing();
}
