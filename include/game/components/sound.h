#pragma once
#include "system/asset_manager.h"

namespace Game::Audio
{
    struct Sound
    {
        const AssetsRecord *asset = nullptr;
        float volume = 1.0f;
        bool play = false;
    };
} // namespace Game::Audio
