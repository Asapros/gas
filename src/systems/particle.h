#pragma once

#include "system.h"

struct ParticleSystem : System {
    private:
        float last_tick;
        float last_load_flush;

    using System::System;
    void init() override;
    void draw() override;
    void tick() override;
};
