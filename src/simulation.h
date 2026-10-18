#include <memory>
#include <vector>
#include "systems/system.h"

struct Simulation {
    std::vector<std::unique_ptr<System>> systems;
    GameState state;

    Simulation();


    void init();
    void tick();
    void draw();
};


