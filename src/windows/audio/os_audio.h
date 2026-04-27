#pragma once
#include "audio.h"
#include "system/asset_manager.h"
#include "system/audio.h"
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
    char active;
    char is_loop;
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
    AudioQueue queue;
} AudioAPI;

typedef void *AudioHandler;

HRESULT audio_init(AudioHandler *out_handler);
void audio_handler_cleanup(_In_ AudioHandler *handler);
