#include "particle.h"
#include "../rng.h"
#include <raylib.h>
#include <raymath.h>

void ParticleSystem::init() {
    this->state->container_offset = {560, 0};
    this->state->container_size = {720, 720};

    for (int i = 0; i < 1000; i++) {
        float x = this->state->container_offset.x + randomFloat(0, this->state->container_size.x);
        float y = this->state->container_offset.y + randomFloat(0, this->state->container_size.y);
        float direction = randomFloat(0, 2 * PI);
        float speed = 300.0;
        this->state->particles.emplace_back(Particle { Vector2 { x, y }, Vector2Scale(Vector2 { cosf(direction), sinf(direction) }, speed), 3.35e-27 });
    }
    last_tick = GetTime();
}

void ParticleSystem::draw() {
    for (Particle particle : this->state->particles) {
        DrawCircle(particle.position.x, particle.position.y, 3.0, RED);
    }
}

void ParticleSystem::tick() {
    float time_delta = GetTime() - this->last_tick;
    double total_collision_momentum = 0;
    for (Particle &particle : this->state->particles) {
        Vector2 displacement = Vector2Scale(particle.velocity, time_delta);
        particle.position = Vector2Add(particle.position, displacement);

        if (particle.position.x < this->state->container_offset.x) {
            particle.position.x += 2 * (this->state->container_offset.x - particle.position.x);
            particle.velocity.x = -particle.velocity.x;
            total_collision_momentum += 2 * abs(particle.velocity.x) * particle.mass;
        }
        else if (particle.position.x > this->state->container_offset.x + this->state->container_size.x) {
            particle.position.x += 2 * (this->state->container_offset.x + this->state->container_size.x - particle.position.x);
            particle.velocity.x = -particle.velocity.x;
            total_collision_momentum += 2 * abs(particle.velocity.x) * particle.mass;
        }
        if (particle.position.y < this->state->container_offset.y) {
            particle.position.y += 2 * (this->state->container_offset.y - particle.position.y);
            particle.velocity.y = -particle.velocity.y;
            total_collision_momentum += 2 * abs(particle.velocity.y) * particle.mass;
        }
        else if (particle.position.y > this->state->container_offset.y + this->state->container_size.y) {
            particle.position.y += 2 * (this->state->container_offset.y + this->state->container_size.y - particle.position.y);
            particle.velocity.y = -particle.velocity.y;
            total_collision_momentum += 2 * abs(particle.velocity.y) * particle.mass;
        }
    }
    this->last_tick = GetTime();
    this->state->last_tick_collision_momentum = total_collision_momentum;
}
