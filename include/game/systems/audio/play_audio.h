#pragma once
#include "game/components/audio/audio.h"
#include "system.h"
#include "utils/print_debug.h"

namespace Game::Audio
{
    template<typename T>
    void play_audio([[maybe_unused]] T &syscall, System::ECS::Query<Game::Audio::Audio> &query)
    {
        for (auto &[id, comp]: query)
        {
            auto &audio = comp.get<Game::Audio::Audio>();

            if (!audio.has_played)
            {
                audio_play(audio.audio, audio.volume, static_cast<char>(audio.is_loop));
                audio.has_played = true; // Prevent it from playing again next frame!
            }
        }
    }
} // namespace Game::Audio
