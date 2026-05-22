#pragma once
#include "system/asset_manager.h"
#include "system/atomic.h"

#define AUDIO_QUEUE_SIZE 256

typedef struct
{
    void *pcm_data;
    size_t total_frames;
    float volume;
    char is_loop;
} AudioCommand;

typedef struct
{
    AudioCommand buffer[AUDIO_QUEUE_SIZE];
    atomic_size_t head;
    atomic_size_t tail;
} AudioQueue;

typedef struct AudioCache
{
    float *pcm_data;
    unsigned long long frame_count;
    int channels;
    int sample_rate;
} AudioCache;

#ifdef __cplusplus
extern "C"
{
#endif
    void *get_audio_queue();
    int load_audio_if_not_exist(AssetsRecord *rec, AudioCache **out);
    void audio_play(const AssetsRecord *record, float volume, char is_loop);

#ifdef __cplusplus
}
#endif
