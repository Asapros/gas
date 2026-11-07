#pragma once

#include "system.h"
#include <raylib.h>

enum Setting {
    CONTAINER_WIDTH,
    CONTAINER_HEIGHT,
    CONTAINER_DEPTH,
    PARTICLE_NUMBER,
    VIEW_DIMENSIONS
};

enum Language {
    POLISH,
    ENGLISH
};

struct UiSystem : System {
    using System::System;
    private:
        Font font;
        bool setting_selected;
        size_t current_setting;
        Language language;
    public:
    void after_draw() override;
    void tick() override;
    void init() override;
};
