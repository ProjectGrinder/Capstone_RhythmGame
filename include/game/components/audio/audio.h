#pragma once
#include <unordered_map>

#include "audio.h"
#include "system/asset_manager.h"

namespace Game::Audio
{
    struct Audio
    {
        const AssetsRecord *audio;
        int max_cooldown;
        int cooldown = 0;
        float volume;
        bool is_loop = false;
        bool play = false;

        explicit Audio(const AssetsRecord *audio = nullptr, const float volume = 1.0f, const int max_cooldown = 3, bool loop = false) :
            audio(audio), max_cooldown(max_cooldown), volume(volume), is_loop(loop)
        {}
    };

    struct SoundRegistry
    {
        std::unordered_map<std::string, Audio> audios;
    };

} // namespace Game::Audio
