#pragma once

#include "system.h"

struct CalculatorSystem : System {
    private:
        float last_tick;
        float time_since_calculation;
        float collision_momentum_accumulator;
    using System::System;
    void init() override;
    void tick() override;
};
