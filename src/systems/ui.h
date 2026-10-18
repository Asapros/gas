#pragma once

#include "system.h"

struct UiSystem : System {
    using System::System;
    void after_draw() override;
};
