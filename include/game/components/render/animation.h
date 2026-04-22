#pragma once
#include <array>
#include "utils/print_debug.h"

namespace Game::Render
{
    struct Animation
    {
        int current_frame;
        bool isLoop;
    };
} // namespace Game::Render
