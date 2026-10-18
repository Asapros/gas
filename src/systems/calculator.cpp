#include "calculator.h"
#include <raylib.h>
#include <raymath.h>

const double BOLTZMANN_CONSTANT = 1.3806e-23;

void CalculatorSystem::init() {
    this->last_tick = GetTime();
    this->time_since_calculation = 0;
    this->state->calculation_peroid = 2.0;
    this->collision_momentum_accumulator = 0;
    this->collision_accumulator = 0;
}

void CalculatorSystem::tick() {
    float delta = GetTime() - this->last_tick;
    this->last_tick = GetTime();
    this->time_since_calculation += delta;
    this->collision_momentum_accumulator += this->state->last_tick_collision_momentum;
    this->collision_accumulator += this->state->last_tick_collisions;

    if (this->time_since_calculation < this->state->calculation_peroid) {
        return;
    }
    this->time_since_calculation -= this->state->calculation_peroid;
    this->state->average_load = this->collision_momentum_accumulator / this->state->calculation_peroid / this->state->boundary->surface();
    this->collision_momentum_accumulator = 0;
    this->collision_accumulator = 0;
    double total_kinetic_energy = 0;
    for (Particle &particle : this->state->particles) {
        total_kinetic_energy += particle.mass * particle.velocity.length_squared() / 2;
    }
    this->state->average_temperature = total_kinetic_energy / this->state->particles.size() / BOLTZMANN_CONSTANT * this->state->boundary->dimensions() / 2;
}
