#pragma once
#include <array>
#include "utils/print_debug.h"

namespace Game::Render
{
    struct Animator
    {
        Math::Point offset;
        int anim_id;
        int current_frame;
        bool isLoop;
    };
} // namespace Game::Render
