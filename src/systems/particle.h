#pragma once

#include "system.h"
#include <raylib.h>

struct ParticleSystem : System {
    private:
        float last_tick;
        float last_load_flush;
        RenderTexture render_buffer;
        Camera3D camera;

    using System::System;
    void init() override;
    void draw() override;
    void tick() override;
};
