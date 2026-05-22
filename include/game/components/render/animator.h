#pragma once
#include <array>
#include "utils/print_debug.h"
#include "utils/physics_util.h"

namespace Game::Render
{
    struct Animator
    {
        int anim_id;
        // bool isLoop = false;
        int current_frame = 0;
        int current_time = 0;

        Animator(
                const int anim_id = -1): anim_id(anim_id)
        {}
    };

    struct Animation_Controller
    {
        int current_id = 0;
        int to_id = 0;
        bool has_exit_time;
        int transition_time;
    };
} // namespace Game::Render
