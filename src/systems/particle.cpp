#include "particle.h"
#include "../rng.h"
#include <algorithm>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include "../rng.h"

void ParticleSystem::init() {
    this->state->boundary = std::make_unique<CubeBoundary>(VectorN {400, 400 }, VectorN { 0, 0 });

    for (int i = 0; i < 512; i++) {
        VectorN velocity = randomUnitVector(this->state->boundary->dimensions()) * 300.0;
        this->state->particles.emplace_back(Particle { this->state->boundary->randomPoint(), velocity, 1.67e-27 });
    }
    last_tick = GetTime();
}

void ParticleSystem::draw() {
    float viewport_size = std::min(GetScreenWidth(), GetScreenHeight());
    float x_offset = GetScreenWidth() - viewport_size;
    float margin = viewport_size / 40;
    DrawRectangleLines(x_offset, 0, viewport_size, viewport_size, WHITE);

    CubeBoundary* boundary = dynamic_cast<CubeBoundary*>(this->state->boundary.get());
    float scale = (viewport_size - 2 * margin) / std::max(boundary->size[0], boundary->size[1]);

    DrawRectangleLines(margin + x_offset, margin, boundary->size[0]*scale, boundary->size[1]*scale, WHITE);

    for (Particle particle : this->state->particles) {
        DrawCircle(x_offset + margin + particle.position[0] * scale, margin + particle.position[1] * scale, 3 * scale, RED);
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
