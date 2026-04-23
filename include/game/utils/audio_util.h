#pragma once
#include "system/asset_manager.h"
#include "audio.h"

namespace Game::Audio
{
    extern "C" void *get_audio_api(void);
    inline SoundRegistry init_sounds()
    {
        SoundRegistry sound_registry;
        sound_registry.audios.insert({
            {"sound_menu_hover",Audio(load_audio("audio/POP1.wav","sound_menu_hover"))},
            {"sound_menu_click",Audio(load_audio("audio/Click.mp3","sound_menu_click"))},
            {"sound_main_transition",Audio(load_audio("audio/RUN_FLINTSTONE.mp3","sound_main_transition"))},
            {"sound_walk",Audio(load_audio("audio/SHAKER.wav","sound_walk"))},
            {"sound_jump",Audio(load_audio("audio/POP1.wav","sound_jump"))},
            {"sound_run",Audio(load_audio("audio/SHAKER.wav","sound_run"))},
            {"sound_interact",Audio(load_audio("audio/SHARP0.wav","sound_interact"))},
            {"sound_dialogue",Audio(load_audio("audio/Click.mp3","sound_dialogue"))},
            {"sound_bullet_spawn_0",Audio(load_audio("audio/POP1.wav","sound_bullet_spawn_0"))},
            {"sound_bullet_spawn_1",Audio(load_audio("audio/SHARP0.mp3","sound_bullet_spawn_1"))},
            {"sound_bullet_spawn_2",Audio(load_audio("audio/SHAKER.wav","sound_bullet_spawn_2"))},
            {"sound_bullet_laser",Audio(load_audio("audio/Anvil_hit.wav","sound_bullet_laser"))},
            {"sound_bullet_boomer",Audio(load_audio("audio/Anvil_hit.mp3","sound_bullet_boomer"))},
            {"sound_graze",Audio(load_audio("audio/SHARP0.wav","sound_graze"))},
            {"sound_hit",Audio(load_audio("audio/fishdam1.wav","sound_hit"))},
            {"sound_phase_transition",Audio(load_audio("audio/RUN_FLINTSTONE.mp3","sound_phase_transition"))},
            {"sound_note_click",Audio(load_audio("audio/POP1.wav","sound_note_click"))},
            {"sound_note_miss",Audio(load_audio("audio/fishdam1.wav","sound_note_miss"))}
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
