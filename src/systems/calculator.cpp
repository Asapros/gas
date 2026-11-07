#include "calculator.h"
#include <raylib.h>
#include <raymath.h>

void CalculatorSystem::init() {
    this->last_tick = GetTime();
    this->time_since_calculation = 0;
    this->state->calculation_peroid = 3.0;
    this->collision_momentum_accumulator = 0;
    this->collision_accumulator = 0;
}

void CalculatorSystem::tick() {
    float delta = GetTime() - this->last_tick;
    this->last_tick = GetTime();
    this->time_since_calculation += delta;
    this->collision_momentum_accumulator += this->state->last_tick_collision_momentum;
    this->collision_accumulator += this->state->last_tick_collisions;
    
    this->state->total_kinetic_energy = 0;
    float total_speed = 0;
    for (Particle &particle : this->state->particles) {
        total_speed += particle.velocity.length();
        this->state->total_kinetic_energy += particle.mass * particle.velocity.length_squared() / 2;
    }
    this->state->average_speed = total_speed / this->state->particles.size();
    this->state->average_temperature = this->state->total_kinetic_energy / this->state->particles.size() / BOLTZMANN_CONSTANT / this->state->boundary->dimensions() * 2;
    if (this->time_since_calculation < this->state->calculation_peroid) {
        return;
    }
    this->time_since_calculation -= this->state->calculation_peroid;
    this->state->average_collisions = this->collision_accumulator / this->state->calculation_peroid;
    this->state->average_momentum_transferred = this->collision_momentum_accumulator / this->state->calculation_peroid;
    this->state->average_load = this->state->average_momentum_transferred / this->state->boundary->surface();
    this->collision_momentum_accumulator = 0;
    this->collision_accumulator = 0;
}
