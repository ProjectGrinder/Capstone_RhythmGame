#pragma once
#include "system.h"

namespace Game::Audio
{
    struct Audio
    {
        const AssetsRecord *audio = nullptr; // font
        float volume = 0;
        bool is_loop = false;
        bool has_played = false;
    };
} // namespace Game::Audio
