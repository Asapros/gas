#pragma once

#include <raylib.h>
#include <vector>

struct Particle {
    Vector2 position;
    Vector2 velocity;
    double mass;
};

struct GameState {
    Vector2 container_size;
    Vector2 container_offset;
    std::vector<Particle> particles;
    double last_tick_collision_momentum;
    double calculation_peroid;
    double average_load;
    double average_temperature;
};
