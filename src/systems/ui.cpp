#include "ui.h"
#include <raylib.h>
#include "../rng.h"

const int ALL_OPTIONS = 3;

void UiSystem::init() {
    this->current_option = 0;
}

void UiSystem::after_draw() {
    DrawText(TextFormat("FPS = %d", GetFPS()), 0, 0, 32, WHITE);

    DrawText(TextFormat("V = %.0f", this->state->boundary->volume()), 0, 32, 32, WHITE);
    DrawText(TextFormat("p = %.2e", this->state->average_load), 0, 64, 32, WHITE);
    DrawText(TextFormat("T = %f", this->state->average_temperature), 0, 128, 32, WHITE);
    float R = this->state->average_load * this->state->boundary->volume() / this->state->particles.size() * this->state->average_temperature;
    DrawText(TextFormat("pV/nT = %.2e", R), 0, 160, 32, WHITE);

    CubeBoundary* boundary = dynamic_cast<CubeBoundary*>(this->state->boundary.get());
    DrawText(TextFormat("X: < %.0f >", boundary->size.values[0]), 0, 256, 32, this->current_option == 0 ? YELLOW : WHITE);
    DrawText(TextFormat("Y: < %.0f >", boundary->size.values[1]), 0, 288, 32, this->current_option == 1 ? YELLOW : WHITE);
    DrawText(TextFormat("n: < %d >", this->state->particles.size()), 0, 320, 32, this->current_option == 2 ? YELLOW : WHITE);
}

void UiSystem::tick() {
    if (IsKeyPressed(KEY_DOWN)) {
        current_option = (current_option + 1) % ALL_OPTIONS;
    }
    if (IsKeyPressed(KEY_UP)) {
        current_option = (current_option - 1 + ALL_OPTIONS) % ALL_OPTIONS;
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        switch (current_option) {
            case 0: {
                CubeBoundary* boundary = dynamic_cast<CubeBoundary*>(this->state->boundary.get());
                boundary->size[0] += 50;
                break;
            }
            case 1: {
                CubeBoundary* boundary = dynamic_cast<CubeBoundary*>(this->state->boundary.get());
                boundary->size[1] += 50;
                break;
            }
            case 2: {
                int to_spawn = this->state->particles.size();
                for (int i = 0; i < to_spawn; i++) {
                    this->state->particles.emplace_back(Particle {this->state->boundary->randomPoint(), randomUnitVector(this->state->boundary->dimensions()) * 300.0, 1.67e-27});
                }
            }
        }
    }
    if (IsKeyPressed(KEY_LEFT)) {
        switch (current_option) {
            case 0: {
                CubeBoundary* boundary = dynamic_cast<CubeBoundary*>(this->state->boundary.get());
                boundary->size[0] -= 50;
                break;
            }
            case 1: {
                CubeBoundary* boundary = dynamic_cast<CubeBoundary*>(this->state->boundary.get());
                boundary->size[1] -= 50;
                break;
            }
            case 2: {
                if (this->state->particles.size() == 1) break;
                this->state->particles.erase(this->state->particles.begin() + this->state->particles.size() / 2, this->state->particles.end());
                break;
            }
        }
    }
}
