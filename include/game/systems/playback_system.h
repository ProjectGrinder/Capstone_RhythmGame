#include "audio.h"
#include "game/components/sound.h"
#include "system/ecs.h"

namespace Game::Audio
{
    extern "C" void *get_audio_api(void);

    template<typename T>
    inline void playback_system([[maybe_unused]] T &syscall, System::ECS::Query<Game::Audio::Sound> &query)
    {
        void *raw_audio = get_audio_api();
        if (raw_audio == nullptr)
            return;

        AudioAPI *api = static_cast<AudioAPI *>(raw_audio);

        for (auto &[id, comps]: query)
        {
            auto &sound = comps.get<Game::Audio::Sound>();

            if (sound.play && sound.asset != nullptr)
            {
                AudioCache *cache = static_cast<AudioCache *>(sound.asset->gpu_extension);

                if (cache != nullptr)
                {
                    // Find the first available slot in your C-Mixer (assuming you have e.g., 32 slots)
                    // Change the '32' to whatever your MAX_SOUNDS macro is in audio.h!
                    for (int i = 0; i < 32; ++i)
                    {
                        if (api->mixer.sounds[i].active == 0)
                        {
                            api->mixer.sounds[i].pcm_data = cache->pcm_data;
                            api->mixer.sounds[i].total_frames = cache->frame_count;
                            api->mixer.sounds[i].current_frame = 0;
                            api->mixer.sounds[i].volume = sound.volume;
                            api->mixer.sounds[i].active = 1; // FIRE!
                            break;                           // Slot found, stop searching
                        }
                    }
                }

                // Reset the trigger so it only plays once per event
                sound.play = false;
            }
        }
    }
} // namespace Game::Audio
