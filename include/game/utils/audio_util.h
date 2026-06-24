#pragma once
#include "system/asset_manager.h"
#include "audio.h"

namespace Game::Audio
{
    extern "C" void *get_audio_api(void);
    inline SoundRegistry init_battle_sounds(const int level_num)
    {
        std::string bgm = "audio/";
        switch (level_num)
        {
            case 0:
            bgm.append("DemoSong.wav");
            break;
            case 1:
            bgm.append("DemoSong2.wav");
            break;
            default:
            bgm.append("DemoSong.wav");
        }
        const AssetsRecord* sound_ptr[] = {
            load_audio("audio/POP1.wav","POP1"),
            load_audio("audio/Click.mp3","Click"),
            load_audio("audio/RUN_FLINTSTONE.mp3","Run"),
            load_audio("audio/SHAKER.wav","Shaker"),
            load_audio("audio/Sharp.mp3","sound_interact"),
            load_audio("audio/Anvil_hit.wav","sound_bullet_laser"),
            load_audio("audio/fishdam1.wav","sound_hit"),
            load_audio("audio/edm-clap-53.wav","base_note"),
            load_audio("audio/VEC1 Tamb 07.wav","accent_fx"),
            load_audio("audio/edm-hihat-12.wav","rain_note"),
            load_audio(bgm.c_str(),"level_music")
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
            // {"sound_note_click",Audio(sound_ptr[0])},
            // {"sound_note_miss",Audio(sound_ptr[6])},
            {"sound_note_hit", Audio(sound_ptr[7])},
            {"sound_rain_note", Audio(sound_ptr[9])},
            {"sound_accent_effect", Audio(sound_ptr[8])},
            {"level_music", Audio(sound_ptr[10])}
        });

        for (const auto& [key, sound] : sound_registry.audios) {
            AudioCache *out = nullptr;
            load_audio_if_not_exist((AssetsRecord *)sound.audio, &out);
        }

        return sound_registry;
    }

    inline SoundRegistry init_world_sounds()
    {
        const AssetsRecord* sound_ptr[] = {
            load_audio("audio/VEC1 Percussion 022.wav", "click"),
            load_audio("audio/VEC1 Percussion 024.wav", "click2"),
            load_audio("audio/VEC1 Percussion 025.wav", "click3")
        };
        SoundRegistry sound_registry;
        sound_registry.audios.insert({
            {"sound_popup", Audio(sound_ptr[0])},
            {"sound_confirm", Audio(sound_ptr[1])},
            {"sound_select", Audio(sound_ptr[2])}
        });
        return sound_registry;
    }

    inline void audio_play(const Audio &audio)
    {
        audio_play(audio.audio, audio.volume, static_cast<char>(audio.is_loop));
    }
}
