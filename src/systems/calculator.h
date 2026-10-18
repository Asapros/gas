#pragma once

#include "system.h"

const double BOLTZMANN_CONSTANT = 1.3806e-23;
const double AVOGADRO_CONSTANT = 6.022e23;

struct CalculatorSystem : System {
    private:
        float last_tick;
        float time_since_calculation;
        float collision_momentum_accumulator;
        float collision_accumulator;
    using System::System;
    void init() override;
    void tick() override;
};
