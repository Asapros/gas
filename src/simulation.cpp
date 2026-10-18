#include <raylib.h>
#include "simulation.h"
#include "systems/calculator.h"
#include "systems/particle.h"
#include "systems/ui.h"

Simulation::Simulation() {
    this->systems.emplace_back(std::make_unique<ParticleSystem>(this->state));
    this->systems.emplace_back(std::make_unique<CalculatorSystem>(this->state));
    this->systems.emplace_back(std::make_unique<UiSystem>(this->state));
}

void Simulation::init() {
    for (auto& system : this->systems) {
        system->init();
    }
}

void Simulation::tick() {
    for (auto& system : this->systems) {
        system->tick();
    }
}

void Simulation::draw() {
    for (auto& system : this->systems) {
        system->before_draw();
    }
    for (auto& system : this->systems) {
        system->draw();
    }
    for (auto& system : this->systems) {
        system->after_draw();
    }
}
