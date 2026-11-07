#include <cmath>
#include <raylib.h>
#include "ui.h"
#include "../rng.h"

const Setting SETTINGS_CYCLE[] = {
    Setting::CONTAINER_WIDTH,
    Setting::CONTAINER_HEIGHT,
    Setting::CONTAINER_DEPTH,
    Setting::PARTICLE_NUMBER,
    Setting::VIEW_DIMENSIONS
};

void UiSystem::init() {
    this->font = LoadFontEx(
        "resources/lmroman12-regular.otf",
        128,
        0,
        256
    );
    SetTextureFilter(this->font.texture, TEXTURE_FILTER_BILINEAR);
    setting_selected = false;
}

#define FONT_SIZE 52
#define VALUE_SHIFT_CHARACTERS 10

void UiSystem::after_draw() {
    float y = 0;
    DrawTextEx(this->font, "[ Container ]", { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE * 1.5, 0, WHITE);
    y += FONT_SIZE;
    CubeBoundary* boundary = dynamic_cast<CubeBoundary*>(this->state->boundary.get());
    DrawTextEx(this->font, "Shape:", { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, "Rectangular", { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    DrawTextEx(this->font, "Width:", { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == CONTAINER_WIDTH ? YELLOW : WHITE);
    DrawTextEx(this->font, TextFormat("%.0f m", boundary->size[0]), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == CONTAINER_WIDTH ? YELLOW : WHITE);
    DrawTextEx(this->font, "Height:", { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == CONTAINER_HEIGHT ? YELLOW : WHITE);
    DrawTextEx(this->font, TextFormat("%.0f m", boundary->size[1]), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == CONTAINER_HEIGHT ? YELLOW : WHITE);
    DrawTextEx(this->font, "Depth:", { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == CONTAINER_DEPTH ? YELLOW : WHITE);
    DrawTextEx(this->font, TextFormat("%.0f m", boundary->dimensions() >= 3 ? boundary->size[2] : 0), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == CONTAINER_DEPTH ? YELLOW : WHITE);
    DrawTextEx(this->font, "Wall Surface:", { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("%.0f m%s", boundary->surface(), boundary->dimensions() >= 3 ? "²" : ""), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    DrawTextEx(this->font, "Volume:", { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("%.0f m%s", boundary->volume(), boundary->dimensions() >= 3 ? "³" : "²"), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);

    y += FONT_SIZE;
    DrawTextEx(this->font, "[ Particles ]", { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE * 1.5, 0, WHITE);
    y += FONT_SIZE;
    DrawTextEx(this->font, "Mass per particle:", { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("%.2e kg", this->state->particles[0].mass), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    DrawTextEx(this->font, "Particle radius:", { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, "0 (no collisions)", { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    DrawTextEx(this->font, "Number:", { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == PARTICLE_NUMBER ? YELLOW : WHITE);
    DrawTextEx(this->font, TextFormat("%d", this->state->particles.size()), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == PARTICLE_NUMBER ? YELLOW : WHITE);

    DrawTextEx(this->font, "Average velocity:", { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("%.0f m/s", this->state->particles[0].velocity.length()), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    y += FONT_SIZE;
    DrawTextEx(this->font, "[ View ]", { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE * 1.5, 0, WHITE);
    y += FONT_SIZE;
    DrawTextEx(this->font, "Dimensions:", { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == VIEW_DIMENSIONS ? YELLOW : WHITE);
    DrawTextEx(this->font, TextFormat("%d", this->state->view_dimensions), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == VIEW_DIMENSIONS ? YELLOW : WHITE);
    DrawTextEx(this->font, "Scale:", { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("100 px : %.2f m", 100/this->state->scale), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    DrawTextEx(this->font, "Particle radius:", { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, "3 m", { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    DrawTextEx(this->font, "FPS:", { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("%d", GetFPS()), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
}

void UiSystem::tick() {
    if (IsKeyPressed(KEY_DOWN)) {
        this->setting_selected = true;
        this->current_setting = (this->current_setting + 1) % (sizeof(SETTINGS_CYCLE) / sizeof(Setting));
    }
    if (IsKeyPressed(KEY_UP)) {
        this->setting_selected = true;
        this->current_setting = (this->current_setting + (sizeof(SETTINGS_CYCLE) / sizeof(Setting)) - 1) % (sizeof(SETTINGS_CYCLE) / sizeof(Setting));
    }
    if (IsKeyPressed(KEY_SPACE)) {
        this->setting_selected = false;
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        switch (this->current_setting) {
            case CONTAINER_WIDTH: {
                CubeBoundary* boundary = dynamic_cast<CubeBoundary*>(this->state->boundary.get());
                boundary->size[0] += 50;
                break;
            }
            case CONTAINER_HEIGHT: {
                CubeBoundary* boundary = dynamic_cast<CubeBoundary*>(this->state->boundary.get());
                boundary->size[1] += 50;
                break;
            }
            case CONTAINER_DEPTH: {
                CubeBoundary* boundary = dynamic_cast<CubeBoundary*>(this->state->boundary.get());
                if (boundary->dimensions() < 3) {
                    boundary->size = VectorN { boundary->size.values[0], boundary->size.values[1], 0 };
                    boundary->offset = VectorN { 0, 0, 0 };
                    for (Particle &particle : this->state->particles) {
                        double speed = particle.velocity.length();
                        particle.velocity = (VectorN { particle.velocity.values[0], particle.velocity.values[1], randomFloat(0, speed) }).normalized() * speed;
                        particle.position = VectorN { particle.position.values[0], particle.position.values[1], 0 };
                    }
                };
                boundary->size[2] += 50;
                break;
            }
            case PARTICLE_NUMBER: {
                int to_spawn = this->state->particles.size();
                for (int i = 0; i < to_spawn; i++) {
                    this->state->particles.emplace_back(Particle {this->state->boundary->randomPoint(), randomUnitVector(this->state->boundary->dimensions()) * 300.0, 1.67e-27});
                }
                break;
            }
            case VIEW_DIMENSIONS: {
                this->state->view_dimensions = 2 + (this->state->view_dimensions + 1) % 2;
                break;
            }
        }
    }
    if (IsKeyPressed(KEY_LEFT)) {
        switch (this->current_setting) {
            case CONTAINER_WIDTH: {
                CubeBoundary* boundary = dynamic_cast<CubeBoundary*>(this->state->boundary.get());
                if (boundary->size[0] <= 50) break;
                boundary->size[0] -= 50;
                break;
            }
            case CONTAINER_HEIGHT: {
                CubeBoundary* boundary = dynamic_cast<CubeBoundary*>(this->state->boundary.get());
                if (boundary->size[1] <= 50) break;
                boundary->size[1] -= 50;
                break;
            }
            case CONTAINER_DEPTH: {
                CubeBoundary* boundary = dynamic_cast<CubeBoundary*>(this->state->boundary.get());
                if (boundary->dimensions() < 3) break;
                boundary->size[2] -= 50;
                if (boundary->size[2] <= 0) {
                    boundary->size = VectorN { boundary->size[0], boundary->size[1] };
                    boundary->offset = VectorN { 0, 0 };
                    for (Particle &particle : this->state->particles) {
                        double speed = particle.velocity.length();
                        particle.velocity = (VectorN { particle.velocity.values[0], particle.velocity.values[1] }).normalized() * speed;
                        particle.position = VectorN { particle.position.values[0], particle.position.values[1] };
                    }
                };
                break;
            }
            case PARTICLE_NUMBER: {
                if (this->state->particles.size() == 1) break;
                this->state->particles.erase(this->state->particles.begin() + this->state->particles.size() / 2, this->state->particles.end());
                break;
            }
            case VIEW_DIMENSIONS: {
                this->state->view_dimensions = 2 + (this->state->view_dimensions + 1) % 2;
                break;
            }
        }
    }
}
