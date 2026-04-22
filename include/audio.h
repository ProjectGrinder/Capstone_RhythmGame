#pragma once
#include "system/asset_manager.h"
#pragma warning(push)
#pragma warning(disable : 4244)
#pragma warning(disable : 4220)
#include "library/miniaudio.h"
#pragma warning(pop)

#include <Windows.h>

#define MAX_ACTIVE_SOUNDS 64

typedef struct PlayingSound
{
    float *pcm_data;
    unsigned long long total_frames;
    unsigned long long current_frame;
    float volume;
    char active; // 1 = playing, 0 = free slot
} PlayingSound;

typedef struct AudioMixerState
{
    PlayingSound sounds[MAX_ACTIVE_SOUNDS];
    size_t next_slot;
} AudioMixerState;

typedef struct AudioAPI
{
    ma_device device;
    AudioMixerState mixer;
} AudioAPI;

typedef void *AudioHandler;

typedef struct AudioCache
{
    float *pcm_data;
    unsigned long long frame_count;
    int channels;
    int sample_rate;
} AudioCache;

#ifdef __cplusplus
extern "C" {
#endif
HRESULT audio_init(AudioHandler *out_handler);
void audio_handler_cleanup(_In_ AudioHandler *handler);
int load_audio_if_not_exist(AssetsRecord *rec, AudioCache **out);
#ifdef __cplusplus
}
#endif