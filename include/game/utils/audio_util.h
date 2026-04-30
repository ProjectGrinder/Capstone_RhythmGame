#pragma once
#include "system/asset_manager.h"
#include "audio.h"

namespace Game::Audio
{
    extern "C" void *get_audio_api(void);
    inline SoundRegistry init_sounds()
    {
        const AssetsRecord* sound_ptr[] = {
            load_audio("audio/POP1.wav","POP1"),
            load_audio("audio/Click.mp3","Click"),
            load_audio("audio/RUN_FLINTSTONE.mp3","Run"),
            load_audio("audio/SHAKER.wav","Shaker"),
            load_audio("audio/Sharp.mp3","sound_interact"),
            load_audio("audio/Anvil_hit.wav","sound_bullet_laser"),
            load_audio("audio/fishdam1.wav","sound_hit")
        };
        SoundRegistry sound_registry;
        sound_registry.audios.insert({
            {"sound_menu_hover",Audio(sound_ptr[0])},
            {"sound_menu_click",Audio(sound_ptr[1])},
            {"sound_main_transition",Audio(sound_ptr[2])},
            {"sound_walk",Audio(sound_ptr[3])},
            {"sound_jump",Audio(sound_ptr[0])},
            {"sound_run",Audio(sound_ptr[3])},
            {"sound_interact",Audio(sound_ptr[4])},
            {"sound_dialogue",Audio(sound_ptr[1])},
            {"sound_bullet_spawn_0",Audio(sound_ptr[0])},
            {"sound_bullet_spawn_1",Audio(sound_ptr[1])},
            {"sound_bullet_spawn_2",Audio(sound_ptr[3])},
            {"sound_bullet_laser",Audio(sound_ptr[5])},
            {"sound_bullet_boomer",Audio(sound_ptr[5])},
            {"sound_graze",Audio(sound_ptr[4])},
            {"sound_hit",Audio(sound_ptr[6])},
            {"sound_phase_transition",Audio(sound_ptr[2])},
            {"sound_note_click",Audio(sound_ptr[0])},
            {"sound_note_miss",Audio(sound_ptr[6])}
        });

        for (const auto& [key, sound] : sound_registry.audios) {
            AudioCache *out = nullptr;
            load_audio_if_not_exist((AssetsRecord *)sound.audio, &out);
        }

        return sound_registry;
    }

    inline void audio_play(const Audio audio)
    {
        audio_play(audio.audio, audio.volume, static_cast<char>(audio.is_loop));
    }
}
