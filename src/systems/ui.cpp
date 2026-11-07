#include <cmath>
#include <raylib.h>
#include "ui.h"
#include "../rng.h"
#include "calculator.h"

const Setting SETTINGS_CYCLE[] = {
    Setting::CONTAINER_WIDTH,
    Setting::CONTAINER_HEIGHT,
    Setting::CONTAINER_DEPTH,
    Setting::PARTICLE_NUMBER,
    Setting::VIEW_DIMENSIONS
};

struct Translation {
    std::string name;
    std::string english;
    std::string polish;
};

const Translation TRANSLATIONS[] = {
    { "error.translation", "???", "???" },
    { "label.shape", "Shape:", "Kształt:" },
    { "label.width", "Width:", "Szerokość:" },
    { "label.height", "Height:", "Wysokość:" },
    { "label.depth", "Depth:", "Głębokość:" },
    { "label.wall_surface", "Wall Surface:", "Powierzchnia ścian:" },
    { "label.volume", "Volume:", "Objętość:" },
    { "section.container", "[ Container ]", "[ Pojemnik ]" },
    { "section.particles", "[ Particles ]", "[ Cząsteczki ]" },
    { "section.view", "[ View ]", "[ Widok ]" },
    { "section.measurements", "[ Measurements ]", "[ Pomiary ]" },
    { "section.help", "[ Help ]", "[ Pomoc ]" },
    { "label.mass_per_particle", "Mass per particle:", "Masa cząsteczki:" },
    { "label.particle_radius", "Particle radius:", "Promień cząsteczki:" },
    { "label.number", "Number of particles:", "Liczba cząsteczek:" },
    { "label.number_of_moles", "Number of moles:", "Liczba moli:" },
    { "label.average_velocity", "Average velocity:", "Średnia prędkość:" },
    { "label.total_kinetic_energy", "Total kinetic energy:", "Całk. energia kinetyczna:" },
    { "label.temperature", "Temperature:", "Temperatura:" },
    { "label.dimensions", "Dimensions:", "Wymiary:" },
    { "label.scale", "Scale:", "Skala:" },
    { "label.fps", "FPS:", "FPS:" },
    { "label.averaging_period", "Averaging period:", "Okres uśredniania:" },
    { "label.collision_frequency", "Collision frequency:", "Częstotliwość zderzeń:" },
    { "label.momentum_transfer", "Momentum transfer:", "Przekazywany pęd:" },
    { "label.pressure", "Pressure:", "Ciśnienie:" },
    { "label.pvt", "pV/nT:", "pV/nT:" },
    { "help.arrow_keys", "Select and alter settings with arrow keys", "Aby zmienić ustawienia, użyj strzałek" },
    { "help.language", "Press L to change the language", "Naciśnij L, aby zmienić język" },
    { "help.2d", "Set Depth = 0 to simulate in 2D", "Ustaw Głębokość = 0, aby symulować w 2D" },
    { "value.rectangular", "Rectangular", "Prostokątny" },
    { "value.no_collisions", "0 (no collisions)", "0 (brak zderzeń)" }
};

const char *get_translation(std::string name, Language language) {
    size_t index = 0;
    for (size_t i = 0; i < sizeof(TRANSLATIONS) / sizeof(Translation); i++) {
        if (TRANSLATIONS[i].name == name) {
            index = i;
            break;
        }
    }
    switch (language) {
        case POLISH:
            return TRANSLATIONS[index].polish.c_str();
        case ENGLISH:
            return TRANSLATIONS[index].english.c_str();
    }
}

void UiSystem::init() {
    std::vector<int> codepoints(128);

    for (int i = 0; i < 128; i++) {
        codepoints[i] = i;
    }

    std::vector<int> extra = {
        // Polish characters
        0x0105, 0x0107, 0x0119, 0x0142,
        0x0144, 0x00F3, 0x015B, 0x017A, 0x017C,
        // Polish characters (uppercase)
        0x0104, 0x0106, 0x0118, 0x0141,
        0x0143, 0x00D3, 0x015A, 0x0179, 0x017B,

        // Superscript numbers
        0x2070, 0x00B9, 0x00B2, 0x00B3,
        0x2074, 0x2075, 0x2076, 0x2077,
        0x2078, 0x2079
    };

    codepoints.insert(codepoints.end(), extra.begin(), extra.end());

    this->font = LoadFontEx(
        "resources/lmroman12-regular.otf",
        128,
        codepoints.data(),
        static_cast<int>(codepoints.size())
    );
    SetTextureFilter(this->font.texture, TEXTURE_FILTER_BILINEAR);
    setting_selected = false;
    this->language = POLISH;
}

#define FONT_SIZE 52
#define VALUE_SHIFT_CHARACTERS 10
#define COLUMN_SHIFT_CHARACTERS 16

void UiSystem::after_draw() {

    float y = 0;
    DrawTextEx(this->font, get_translation("section.container", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE * 1.5, 0, WHITE);
    y += FONT_SIZE;
    CubeBoundary* boundary = dynamic_cast<CubeBoundary*>(this->state->boundary.get());
    DrawTextEx(this->font, get_translation("label.shape", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, get_translation("value.rectangular", this->language), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    DrawTextEx(this->font, get_translation("label.width", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == CONTAINER_WIDTH ? YELLOW : WHITE);
    DrawTextEx(this->font, TextFormat("%.0f m", boundary->size[0]), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == CONTAINER_WIDTH ? YELLOW : WHITE);
    DrawTextEx(this->font, get_translation("label.height", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == CONTAINER_HEIGHT ? YELLOW : WHITE);
    DrawTextEx(this->font, TextFormat("%.0f m", boundary->size[1]), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == CONTAINER_HEIGHT ? YELLOW : WHITE);
    DrawTextEx(this->font, get_translation("label.depth", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == CONTAINER_DEPTH ? YELLOW : WHITE);
    DrawTextEx(this->font, TextFormat("%.0f m", boundary->dimensions() >= 3 ? boundary->size[2] : 0), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == CONTAINER_DEPTH ? YELLOW : WHITE);
    DrawTextEx(this->font, get_translation("label.wall_surface", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("%.0f m%s", boundary->surface(), boundary->dimensions() >= 3 ? "²" : ""), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    DrawTextEx(this->font, get_translation("label.volume", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("%.0f m%s", boundary->volume(), boundary->dimensions() >= 3 ? "³" : "²"), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);

    y += FONT_SIZE;
    DrawTextEx(this->font, get_translation("section.particles", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE * 1.5, 0, WHITE);
    y += FONT_SIZE;
    DrawTextEx(this->font, get_translation("label.mass_per_particle", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("%.2e kg", this->state->particles[0].mass), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    DrawTextEx(this->font, get_translation("label.particle_radius", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, get_translation("value.no_collisions", this->language), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    DrawTextEx(this->font, get_translation("label.number", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == PARTICLE_NUMBER ? YELLOW : WHITE);
    DrawTextEx(this->font, TextFormat("%d", this->state->particles.size()), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == PARTICLE_NUMBER ? YELLOW : WHITE);
    DrawTextEx(this->font, get_translation("label.number_of_moles", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("%.2e", this->state->particles.size()/AVOGADRO_CONSTANT), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);

    DrawTextEx(this->font, get_translation("label.average_velocity", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("%.0f m/s", this->state->particles[0].velocity.length()), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    DrawTextEx(this->font, get_translation("label.total_kinetic_energy", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("%.2e J", this->state->total_kinetic_energy), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    DrawTextEx(this->font, get_translation("label.temperature", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("%.1f K", this->state->average_temperature), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    y += FONT_SIZE;
    DrawTextEx(this->font, get_translation("section.view", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE * 1.5, 0, WHITE);
    y += FONT_SIZE;
    DrawTextEx(this->font, get_translation("label.dimensions", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == VIEW_DIMENSIONS ? YELLOW : WHITE);
    DrawTextEx(this->font, TextFormat("%d", this->state->view_dimensions), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, SETTINGS_CYCLE[this->current_setting] == VIEW_DIMENSIONS ? YELLOW : WHITE);
    DrawTextEx(this->font, get_translation("label.scale", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("100 px : %.2f m", 100/this->state->scale), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    DrawTextEx(this->font, get_translation("label.particle_radius", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, "3 m", { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    DrawTextEx(this->font, get_translation("label.fps", this->language), { FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("%d", GetFPS()), { FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);

    y = 0;
    DrawTextEx(this->font, get_translation("section.measurements", this->language), { FONT_SIZE * COLUMN_SHIFT_CHARACTERS + FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE * 1.5, 0, WHITE);
    y += FONT_SIZE;
    DrawTextEx(this->font, get_translation("label.averaging_period", this->language), { FONT_SIZE * COLUMN_SHIFT_CHARACTERS + FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("%.1f s", this->state->calculation_peroid), { FONT_SIZE * COLUMN_SHIFT_CHARACTERS + FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    DrawTextEx(this->font, get_translation("label.collision_frequency", this->language), { FONT_SIZE * COLUMN_SHIFT_CHARACTERS + FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("%.0f Hz", this->state->average_collisions), { FONT_SIZE * COLUMN_SHIFT_CHARACTERS + FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    DrawTextEx(this->font, get_translation("label.momentum_transfer", this->language), { FONT_SIZE * COLUMN_SHIFT_CHARACTERS + FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("%.2e N", this->state->average_momentum_transferred), { FONT_SIZE * COLUMN_SHIFT_CHARACTERS + FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    DrawTextEx(this->font, get_translation("label.pressure", this->language), { FONT_SIZE * COLUMN_SHIFT_CHARACTERS + FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("%.2e N/m%s", this->state->average_load, this->state->boundary->dimensions() >= 3 ? "²" : ""), { FONT_SIZE * COLUMN_SHIFT_CHARACTERS + FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    DrawTextEx(this->font, get_translation("label.pvt", this->language), { FONT_SIZE * COLUMN_SHIFT_CHARACTERS + FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, TextFormat("%.2f J/K*mol", this->state->average_load * this->state->boundary->volume() / this->state->particles.size() * AVOGADRO_CONSTANT / this->state->average_temperature), { FONT_SIZE * COLUMN_SHIFT_CHARACTERS + FONT_SIZE * VALUE_SHIFT_CHARACTERS, y }, FONT_SIZE, 0, LIGHTGRAY);
    y += FONT_SIZE * 1.5;
    y += FONT_SIZE;
    DrawTextEx(this->font, get_translation("section.help", this->language), { FONT_SIZE * COLUMN_SHIFT_CHARACTERS + FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE * 1.5, 0, WHITE);
    y += FONT_SIZE;
    DrawTextEx(this->font, get_translation("help.arrow_keys", this->language), { FONT_SIZE * COLUMN_SHIFT_CHARACTERS + FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, get_translation("help.2d", this->language), { FONT_SIZE * COLUMN_SHIFT_CHARACTERS + FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
    DrawTextEx(this->font, get_translation("help.language", this->language), { FONT_SIZE * COLUMN_SHIFT_CHARACTERS + FONT_SIZE * 1.5, y += FONT_SIZE * 1.5 }, FONT_SIZE, 0, WHITE);
}

void UiSystem::tick() {
    if (IsKeyPressed(KEY_DOWN)) {
        this->current_setting = (this->current_setting + 1) % (sizeof(SETTINGS_CYCLE) / sizeof(Setting));
    }
    if (IsKeyPressed(KEY_UP)) {
        this->current_setting = (this->current_setting + (sizeof(SETTINGS_CYCLE) / sizeof(Setting)) - 1) % (sizeof(SETTINGS_CYCLE) / sizeof(Setting));
    }
    if (IsKeyPressed(KEY_L)) {
        switch (this->language) {
            case POLISH:
                this->language = ENGLISH;
                break;
            case ENGLISH:
                this->language = POLISH;
                break;
        }
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

                        VectorN xy = VectorN{
                            particle.velocity.values[0],
                            particle.velocity.values[1]
                        }.normalized();

                        float z = randomFloat(-1.0, 1.0);
                        float xy_scale = sqrt(1.0 - z*z);

                        particle.velocity = VectorN {
                            xy.values[0] * xy_scale,
                            xy.values[1] * xy_scale,
                            z
                        } * speed;
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
