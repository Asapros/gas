#include "ui.h"
#include <raylib.h>

void UiSystem::after_draw() {
    DrawText(TextFormat("FPS = %d", GetFPS()), 0, 0, 32, WHITE);

    DrawText(TextFormat("V = %.0f", this->state->boundary->volume()), 0, 32, 32, WHITE);
    DrawText(TextFormat("p = %.10e", this->state->average_load), 0, 64, 32, WHITE);
    DrawText(TextFormat("n = %d", this->state->particles.size()), 0, 96, 32, WHITE);
    DrawText(TextFormat("T = %f", this->state->average_temperature), 0, 128, 32, WHITE);
    float R = this->state->average_load * this->state->boundary->volume() / this->state->particles.size() * this->state->average_temperature;
    DrawText(TextFormat("pV/nT = %.10e", R), 0, 160, 32, WHITE);

}
