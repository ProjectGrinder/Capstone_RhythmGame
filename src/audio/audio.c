#include "system/audio.h"
#include "system/asset_manager.h"
#include "utils/print_debug.h"

static AudioQueue queue = {0};

int audio_queue_push(AudioQueue *q, AudioCommand command)
{
    size_t current_head = ATOMIC_LOAD(&q->head);
    size_t next_head = (current_head + 1) % AUDIO_QUEUE_SIZE;
    size_t check_head = ATOMIC_LOAD(&q->tail);

    if (next_head == check_head)
    {
        return 1;
    }

    q->buffer[current_head] = command;

    ATOMIC_STORE(&q->head, next_head);
    return 0;
}

int audio_queue_pop(AudioQueue *q, AudioCommand *out_cmd)
{
    size_t current_tail = ATOMIC_LOAD(&q->tail);
    size_t check_head = ATOMIC_LOAD(&q->head);

    if (current_tail == check_head)
    {
        return 1;
    }

    out_cmd->pcm_data = q->buffer[current_tail].pcm_data;
    out_cmd->total_frames = q->buffer[current_tail].total_frames;
    out_cmd->volume = q->buffer[current_tail].volume;
    out_cmd->is_loop = q->buffer[current_tail].is_loop;

    size_t next_tail = (current_tail + 1) % AUDIO_QUEUE_SIZE;
    ATOMIC_STORE(&q->tail, next_tail);
    return 0;
}

void audio_play(const AssetsRecord *record, float volume, char is_loop)
{
    AudioCache *cache = NULL;
    AudioCommand command;

    if (record->data == NULL)
    {
        LOG_ERROR("Audio asset slot is empty or has been freed!");
        goto exit;
    }

    if (record->info.type != AUDIO)
    {
        LOG_ERROR("Asset %s is not an audio file!", record->info.name);
        goto exit;
    }

    load_audio_if_not_exist((AssetsRecord *) record, &cache);

    if (cache == NULL)
    {
        LOG_ERROR("Unable to load audio %s", record->info.name);
        goto exit;
    }

    command.is_loop = is_loop;
    command.volume = volume;
    command.pcm_data = cache->pcm_data;
    command.total_frames = cache->frame_count;

    audio_queue_push(&queue, command);

exit:
    return;
}

int audio_system_pull_command(AudioCommand *out_cmd)
{
    return audio_queue_pop(&queue, out_cmd);
}
