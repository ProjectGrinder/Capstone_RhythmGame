#pragma once
#include "system/asset_manager.h"
#include "audio.h"

namespace Game::Audio
{
    extern "C" void *get_audio_api(void);
    inline SoundRegistry init_sounds()
    {
        SoundRegistry sound_registry;
        // sound_registry.sounds.insert({
        //     {"sound_menu_hover",Sound(load_audio("audio/POP1.wav","sound_menu_hover"))},
        //     {"sound_menu_click",Sound(load_audio("audio/Click.mp3","sound_menu_click"))},
        //     {"sound_main_transition",Sound(load_audio("audio/RUN_FLINTSTONE.mp3","sound_main_transition"))},
        //     {"sound_walk",Sound(load_audio("audio/SHAKER.wav","sound_walk"))},
        //     {"sound_jump",Sound(load_audio("audio/POP1.wav","sound_jump"))},
        //     {"sound_run",Sound(load_audio("audio/SHAKER.wav","sound_run"))},
        //     {"sound_interact",Sound(load_audio("audio/SHARP0.wav","sound_interact"))},
        //     {"sound_dialogue",Sound(load_audio("audio/Click.mp3","sound_dialogue"))},
        //     {"sound_bullet_spawn_0",Sound(load_audio("audio/POP1.wav","sound_bullet_spawn_0"))},
        //     {"sound_bullet_spawn_1",Sound(load_audio("audio/SHARP0.mp3","sound_bullet_spawn_1"))},
        //     {"sound_bullet_spawn_2",Sound(load_audio("audio/SHAKER.wav","sound_bullet_spawn_2"))},
        //     {"sound_bullet_laser",Sound(load_audio("audio/Anvil_hit.wav","sound_bullet_laser"))},
        //     {"sound_bullet_boomer",Sound(load_audio("audio/Anvil_hit.mp3","sound_bullet_boomer"))},
        //     {"sound_graze",Sound(load_audio("audio/SHARP0.wav","sound_graze"))},
        //     {"sound_hit",Sound(load_audio("audio/fishdam1.wav","sound_hit"))},
        //     {"sound_phase_transition",Sound(load_audio("audio/RUN_FLINTSTONE.mp3","sound_phase_transition"))},
        //     {"sound_note_click",Sound(load_audio("audio/POP1.wav","sound_note_click"))},
        //     {"sound_note_miss",Sound(load_audio("audio/fishdam1.wav","sound_note_miss"))}
        // });
        //
        // for (const auto& [key, sound] : sound_registry.sounds) {
        //     AudioCache *out = nullptr;
        //     load_audio_if_not_exist((AssetsRecord *)sound.asset, &out);
        // }

        return sound_registry;
    }

    inline void sound_play(const Sound &sound)
    {
        if (sound.asset != nullptr)
            return;
        // void *raw_audio = get_audio_api();
        // if (raw_audio == nullptr)
        //     return;
        //
        // AudioAPI *api = static_cast<AudioAPI *>(raw_audio);
        // if (sound.asset != nullptr)
        // {
        //
        //     if (auto cache = static_cast<AudioCache *>(sound.asset->gpu_extension); cache != nullptr)
        //     {
        //         for (int i = 0; i < MAX_ACTIVE_SOUNDS; ++i)
        //         {
        //             if (api->mixer.sounds[i].active == 0)
        //             {
        //                 api->mixer.sounds[i].pcm_data = cache->pcm_data;
        //                 api->mixer.sounds[i].total_frames = cache->frame_count;
        //                 api->mixer.sounds[i].current_frame = 0;
        //                 api->mixer.sounds[i].volume = sound.volume;
        //                 api->mixer.sounds[i].active = 1;
        //                 break;
        //             }
        //         }
        //     }
        // }

    }
}
