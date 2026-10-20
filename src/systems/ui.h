#pragma once

#include "system.h"

struct UiSystem : System {
    private:
        int current_group;
        int current_option;
    using System::System;
    void after_draw() override;
    void tick() override;
    void init() override;
};
