#pragma once

#include "boundary.h"
#include <memory>
#include <raylib.h>
#include <vector>

struct Particle {
    VectorN position;
    VectorN velocity;
    double mass;
};

struct GameState {
    std::unique_ptr<Boundary> boundary;
    std::vector<Particle> particles;
    double last_tick_collision_momentum;
    double calculation_peroid;
    double average_load;
    double average_temperature;
    double scale;
    int last_tick_collisions;
    int view_dimensions;
};
