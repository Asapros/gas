#pragma once

#include "../state.h"

struct System {
    GameState* state;
    
    System(GameState& state) { this->state = &state; }
    virtual ~System() = default;

    virtual void init() {};
    virtual void tick() {};
    virtual void before_draw() {};
    virtual void draw() {};
    virtual void after_draw() {};
};
