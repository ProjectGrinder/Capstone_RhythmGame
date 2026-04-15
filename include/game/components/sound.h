#pragma once
#include <unordered_map>

#include "audio.h"
#include "system/asset_manager.h"

namespace Game::Audio
{
    struct Sound
    {
        const AssetsRecord *asset;
        int max_cooldown;
        int cooldown = 0;
        float volume;
        bool play = false;

        explicit Sound(const AssetsRecord *asset = nullptr, const float volume = 1.0f, const int max_cooldown = 3) : asset(asset), max_cooldown(max_cooldown), volume(volume)
        {}
    };

    struct SoundRegistry
    {
        std::unordered_map<std::string, Sound> sounds;
    };

} // namespace Game::Audio
