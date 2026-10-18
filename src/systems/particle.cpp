#include "particle.h"
#include "../rng.h"
#include <memory>
#include <raylib.h>
#include <raymath.h>

void ParticleSystem::init() {
    this->state->boundary = std::make_unique<CubeBoundary>(VectorN {600, 600, 600}, VectorN { 560, 0, 0 });

    for (int i = 0; i < 1000; i++) {
        VectorN direction(this->state->boundary->dimensions());
        for (std::size_t dimension = 0; dimension < this->state->boundary->dimensions(); dimension++) {
            direction[dimension] = randomFloat(-1, 1);
        }
        VectorN velocity = direction.normalized() * 300.0;
        this->state->particles.emplace_back(Particle { this->state->boundary->randomPoint(), velocity, 1.67e-27 });
    }
    last_tick = GetTime();
}

void ParticleSystem::draw() {
    for (Particle particle : this->state->particles) {
        DrawCircle(particle.position[0], particle.position[1], particle.position[2] / 100, RED);
    }
}

void ParticleSystem::tick() {
    float time_delta = GetTime() - this->last_tick;
    this->last_tick = GetTime();
    int total_collisions = 0;
    double total_collision_momentum = 0;
    for (Particle &particle : this->state->particles) {
        VectorN displacement = particle.velocity * time_delta;

        VectorN correction = this->state->boundary->bounce(particle.position + displacement);
        particle.position = particle.position + displacement + correction;

        for (std::size_t i = 0; i < correction.dimensions(); ++i) {
            if (correction.values[i] == 0.0) {
                continue;
            }
            particle.velocity.values[i] *= -1;
            total_collision_momentum += particle.mass * abs(particle.velocity.values[i]) * 2;
            total_collisions++;
        }
    }
    this->state->last_tick_collision_momentum = total_collision_momentum;
    this->state->last_tick_collisions = total_collisions;
}
