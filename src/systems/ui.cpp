#include "ui.h"
#include <raylib.h>
#include <string>
#include "../rng.h"
#include "calculator.h"

const int ALL_OPTIONS = 4;

void UiSystem::init() {
    this->current_option = 0;
}

const int FONT_SIZE = 32;
const int AREA_PADDING = 32;
const int AREA_MARGIN = 32;

void draw_area(Vector2 offset, std::string title, std::string text, Vector2 size) {
    DrawText(title.c_str(), offset.x + AREA_PADDING, offset.y + AREA_PADDING, FONT_SIZE, WHITE);
    int line_count = 0;

    char** lines = TextSplit(text.c_str(), '\n', &line_count);
    for (int i = 0; i < line_count; i++) {
        DrawText(lines[i], offset.x + AREA_PADDING, offset.y + 2 * AREA_PADDING + FONT_SIZE + i * FONT_SIZE, FONT_SIZE, WHITE);
    }
    DrawRectangleLines(offset.x, offset.y, size.x, size.y, WHITE);
}


void UiSystem::after_draw() {
    float height = GetScreenHeight();
    float width = GetScreenWidth() - height;
    draw_area({AREA_MARGIN, AREA_MARGIN}, "Simulation State", TextFormat("FPS = %d", GetFPS()), {(float) (width / 2 - 2 * AREA_MARGIN), (float) (height / 4 - 2 * AREA_MARGIN)});
    draw_area({(float)((float) width / 2 + AREA_MARGIN), AREA_MARGIN}, "Measurements", TextFormat(
        "p = %.2e", this->state->average_load
    ), {(float) (width / 2 - 2 * AREA_MARGIN), (float) (height / 4 - 2 * AREA_MARGIN)});
    
    draw_area({AREA_MARGIN, AREA_MARGIN + height / 4}, "Container Settings",
        TextFormat(
            "Dimensions: < %d >\nShape: < Rectangle >\nX: < 400 >\nY: < 400 >\nZ: < 400 >"
    ), {(float) (width / 2 - 2 * AREA_MARGIN), (float) (height / 4 - 2 * AREA_MARGIN)});
    draw_area({(float)((float) width / 2 + AREA_MARGIN), AREA_MARGIN + height / 4}, "Particle Settings", TextFormat(
        "Number of particles: < 512 >\nParticle speed: < 300 >\nParticle mass: < H_2 >"
    ), {(float) (width / 2 - 2 * AREA_MARGIN), (float) (height / 4 - 2 * AREA_MARGIN)});

    // DrawText(TextFormat("V = %.0f", this->state->boundary->volume()), 0, 32, 32, WHITE);
    // DrawText(TextFormat("p = %.2e", this->state->average_load), 0, 64, 32, WHITE);
    // DrawText(TextFormat("T = %f K", this->state->average_temperature), 0, 128, 32, WHITE);
    // float R = this->state->average_load * this->state->boundary->volume() / (this->state->particles.size() / AVOGADRO_CONSTANT) * this->state->average_temperature;
    // DrawText(TextFormat("n = %.2e mol", this->state->particles.size() / AVOGADRO_CONSTANT), 0, 160, 32, WHITE);
    // DrawText(TextFormat("pV/nT = %.2e", R), 0, 192, 32, WHITE);
    //
    // CubeBoundary* boundary = dynamic_cast<CubeBoundary*>(this->state->boundary.get());
    // DrawText(TextFormat("X: < %.0f >", boundary->size.values[0]), 0, 256, 32, this->current_option == 0 ? YELLOW : WHITE);
    // DrawText(TextFormat("Y: < %.0f >", boundary->size.values[1]), 0, 288, 32, this->current_option == 1 ? YELLOW : WHITE);
    // DrawText(TextFormat("N: < %d >", this->state->particles.size()), 0, 320, 32, this->current_option == 2 ? YELLOW : WHITE);
    // DrawText(TextFormat("D: < %d >", this->state->view_dimensions), 0, 352, 32, this->current_option == 3 ? YELLOW : WHITE);
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
                break;
            }
            case 3: {
                this->state->view_dimensions = 2 + (this->state->view_dimensions + 1) % 2;
                break;
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
            case 3: {
                this->state->view_dimensions = 2 + (this->state->view_dimensions + 1) % 2;
                break;
            }
        }
    }
}
