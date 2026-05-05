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


    // Not used. Transition time later
    struct Anim_Transition
    {
        int anim_id;
        Anim_Transition(const int anim_id) : anim_id(anim_id) {};
    };
} // namespace Game::Render
