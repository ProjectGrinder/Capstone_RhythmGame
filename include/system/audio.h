#pragma once
#include "system/atomic.h"

#define AUDIO_QUEUE_SIZE 256

typedef struct
{
    void *pcm_data;
    size_t total_frames;
    float volume;
} AudioCommand;

typedef struct
{
    AudioCommand buffer[AUDIO_QUEUE_SIZE];
    atomic_size_t head;
    atomic_size_t tail;
} AudioQueue;

#ifdef __cplusplus
extern "C"
{
#endif

    __forceinline int audio_queue_push(AudioQueue *q, void *pcm, size_t total_frames, float vol)
    {
        size_t current_head = ATOMIC_LOAD(&q->head);
        size_t next_head = (current_head + 1) % AUDIO_QUEUE_SIZE;
        size_t check_head = ATOMIC_LOAD(&q->tail);

        if (next_head == check_head)
        {
            return 0;
        }

        q->buffer[current_head].pcm_data = pcm;
        q->buffer[current_head].total_frames = total_frames;
        q->buffer[current_head].volume = vol;

        ATOMIC_STORE(&q->head, next_head);
        return 1;
    }

    // Returns 1 on success, 0 if queue is empty
    __forceinline int audio_queue_pop(AudioQueue *q, AudioCommand *out_cmd)
    {
        size_t current_tail = ATOMIC_LOAD(&q->tail);
        size_t check_head = ATOMIC_LOAD(&q->head);

        if (current_tail == check_head)
        {
            return 0;
        }

        out_cmd->pcm_data = q->buffer[current_tail].pcm_data;
        out_cmd->total_frames = q->buffer[current_tail].total_frames;
        out_cmd->volume = q->buffer[current_tail].volume;

        size_t next_tail = (current_tail + 1) % AUDIO_QUEUE_SIZE;
        ATOMIC_STORE(&q->tail, next_tail);
        return 1;
    }

#ifdef __cplusplus
}
#endif
