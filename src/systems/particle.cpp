#include "particle.h"
#include "../rng.h"
#include <algorithm>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include "../rng.h"

const int BUFFER_SIZE_3D = 1024;

void ParticleSystem::init() {
    this->state->boundary = std::make_unique<CubeBoundary>(VectorN {400, 400, 400 }, VectorN { 0, 0, 0 });

    for (int i = 0; i < 512; i++) {
        VectorN velocity = randomUnitVector(this->state->boundary->dimensions()) * 300.0;
        this->state->particles.emplace_back(Particle { this->state->boundary->randomPoint(), velocity, 1.67e-27 });
    }
    last_tick = GetTime();

    camera = { 0 };
    camera.position = (Vector3){ 200.0f, 200.0f, 200.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    render_buffer = LoadRenderTexture(BUFFER_SIZE_3D, BUFFER_SIZE_3D);
    this->state->view_dimensions = 2;
}

void ParticleSystem::draw() {
    float viewport_size = std::min(GetScreenWidth(), GetScreenHeight());
    float x_offset = GetScreenWidth() - viewport_size;
    float margin = viewport_size / 40;
    DrawRectangleLines(x_offset, 0, viewport_size, viewport_size, WHITE);

    CubeBoundary* boundary = dynamic_cast<CubeBoundary*>(this->state->boundary.get());
    if (this->state->view_dimensions == 2) {
        float scale = (viewport_size - 2 * margin) / std::max(boundary->size[0], boundary->size[1]);
        this->state->scale = scale;
        
        float visual_x_offset = (viewport_size - 2 * margin - boundary->size[0]*scale)/2;
        float visual_y_offset = (viewport_size - 2 * margin - boundary->size[1]*scale)/2;

        DrawRectangleLines(margin + x_offset + visual_x_offset, margin + visual_y_offset, boundary->size[0]*scale, boundary->size[1]*scale, WHITE);

        for (Particle particle : this->state->particles) {
            DrawCircle(x_offset + margin + particle.position[0] * scale + visual_x_offset, margin + particle.position[1] * scale + visual_y_offset, 3 * scale, RED);
        }
    } else if (this->state->view_dimensions == 3) {
        float distance = Vector3Distance(camera.position, camera.target);
        float fov = camera.fovy * DEG2RAD;

        float visible_size = 2.0f * distance * tanf(fov / 2.0f);

        float cube_width  = boundary->size[0];
        float cube_height = boundary->size[1];
        float cube_depth  = boundary->dimensions() >= 3
                            ? boundary->size[2] : 0.0f;

        float diagonal = sqrtf(
            cube_width * cube_width +
            cube_height * cube_height +
            cube_depth * cube_depth
        );

        float scale = visible_size / diagonal * 0.9;

        this->state->scale = 0.9f * (viewport_size - 2 * margin) / diagonal;

        UpdateCamera(&camera, CAMERA_ORBITAL);

        BeginTextureMode(render_buffer);
            ClearBackground(BLACK);

            BeginMode3D(camera);
                DrawCubeWires(
                    (Vector3){ 0, 0, 0 },
                    cube_width * scale,
                    cube_height * scale,
                    cube_depth * scale,
                    WHITE
                );

                for (Particle particle : this->state->particles) {
                    DrawSphere({
                        (particle.position[0] - cube_width / 2) * scale,
                        (particle.position[1] - cube_height / 2) * scale,
                        ((particle.position.dimensions() >= 3 ? particle.position[2] : 0) - cube_depth / 2) * scale
                    }, 3 * scale, RED);
                }
            EndMode3D();
        EndTextureMode();

        DrawTexturePro(
            render_buffer.texture,
            (Rectangle){
                0, 0,
                (float)BUFFER_SIZE_3D,
                -(float)BUFFER_SIZE_3D
            },
            (Rectangle){
                (float)(x_offset + margin),
                (float)margin,
                (float)(viewport_size - margin - margin),
                (float)(viewport_size - margin - margin)
            },
            (Vector2){ 0, 0 },
            0.0f,
            WHITE
        );
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
