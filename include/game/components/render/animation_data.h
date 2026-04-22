#pragma once
#include <array>
#include "utils/print_debug.h"

namespace Game::Render
{
    struct AnimationData
    {
        int max_frames;
        bool isLoop;
        float duration;
        int loop_frame;
    };

    struct AnimationKeeper
    {
        std::vector<AnimationData> animation_datas;
    };
} // namespace Game::Render
