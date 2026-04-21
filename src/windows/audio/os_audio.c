#define MINIAUDIO_IMPLEMENTATION
#include "./os_audio.h"
#include "../utils/windows_utils.h"
#include "system/asset_manager.h"

int audio_system_pull_command(AudioCommand *out_cmd);

int load_audio_if_not_exist(AssetsRecord *audio_rec, AudioCache **out)
{
    if (audio_rec == NULL)
        return (-1);

    if (audio_rec->gpu_extension != NULL)
    {
        *out = (AudioCache *) audio_rec->gpu_extension;
        return (0);
    }

    ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 2, 48000);
    ma_decoder decoder;

    if (ma_decoder_init_memory(audio_rec->data->data, audio_rec->data->size, &config, &decoder) != MA_SUCCESS)
    {
        LOG_ERROR("Failed to initialize audio decoder from memory.");
        return (-1);
    }

    ma_uint64 total_frames = 0;
    ma_decoder_get_length_in_pcm_frames(&decoder, &total_frames);

    AudioCache *cache = (AudioCache *) heap_alloc(sizeof(AudioCache));
    cache->channels = 2;
    cache->sample_rate = 48000;
    cache->frame_count = total_frames;

    cache->pcm_data = (float *) heap_alloc(total_frames * 2 * sizeof(float));

    ma_uint64 frames_read = 0;
    ma_decoder_read_pcm_frames(&decoder, cache->pcm_data, total_frames, &frames_read);

    ma_decoder_uninit(&decoder);

    audio_rec->gpu_extension = (void *) cache;
    *out = cache;

    return (0);
}

void data_callback(ma_device *device, void *output, const void *input, ma_uint32 frame_count)
{
    (void) input;
    float *fout = (float *) output;

    AudioAPI *api = (AudioAPI *) device->pUserData;
    AudioCommand cmd;

    AudioMixerState *mixer = &api->mixer;

    while (audio_system_pull_command(&cmd) == 0)
    {
        size_t slot = api->mixer.next_slot;

        api->mixer.sounds[slot].pcm_data = cmd.pcm_data;
        api->mixer.sounds[slot].total_frames = cmd.total_frames;
        api->mixer.sounds[slot].current_frame = 0;
        api->mixer.sounds[slot].volume = cmd.volume;
        api->mixer.sounds[slot].active = 1;
        api->mixer.sounds[slot].is_loop = cmd.is_loop;
        api->mixer.next_slot = (slot + 1) % MAX_ACTIVE_SOUNDS;
    }

    for (ma_uint32 i = 0; i < frame_count * device->playback.channels; ++i)
    {
        fout[i] = 0.0f;
    }

    for (int s = 0; s < MAX_ACTIVE_SOUNDS; ++s)
    {
        if (!mixer->sounds[s].active)
            continue;

        PlayingSound *sound = &mixer->sounds[s];

        for (ma_uint32 i = 0; i < frame_count; ++i)
        {
            if (sound->current_frame >= sound->total_frames)
            {
                if (!sound->is_loop)
                {
                    sound->active = 0;
                    break;
                }
                sound->current_frame = 0;
            }

            unsigned long long idx = sound->current_frame * 2;
            fout[i * 2] += sound->pcm_data[idx] * sound->volume;
            fout[i * 2 + 1] += sound->pcm_data[idx + 1] * sound->volume;

            sound->current_frame++;
        }
    }
}

HRESULT audio_init(AudioHandler *out_handler)
{
    if (out_handler == NULL)
        return E_POINTER;

    HRESULT error = S_OK;

    AudioAPI *api = (AudioAPI *) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(AudioAPI));
    if (api == NULL)
    {
        return HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
    }

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_f32;
    config.playback.channels = 2;
    config.sampleRate = 48000;
    config.dataCallback = data_callback;

    config.pUserData = api;

    if (ma_device_init(NULL, &config, &api->device) != MA_SUCCESS)
    {
        LOG_ERROR("Failed to initialize playback device.");
        HeapFree(GetProcessHeap(), 0, api);
        return (E_FAIL);
    }

    if (ma_device_start(&api->device) != MA_SUCCESS)
    {
        LOG_ERROR("Failed to start playback device.");
        ma_device_uninit(&api->device);
        HeapFree(GetProcessHeap(), 0, api);
        return (E_FAIL);
    }

    LOG_INFO("Audio Device Started!");

    *out_handler = (AudioHandler) api;

    return (error);
}

void audio_handler_cleanup(_In_ AudioHandler *handler)
{
    if (handler == NULL || *handler == NULL)
        return;

    AudioAPI *api = (AudioAPI *) (*handler);
    ma_device_uninit(&api->device);

    HeapFree(GetProcessHeap(), 0, api);
    *handler = NULL;
}

void audio_resource_release(void **audio_resource)
{
    AudioCache *cache = (AudioCache *) (*audio_resource);
    if (!cache)
        return;

    if (cache->pcm_data)
    {
        heap_free(cache->pcm_data);
    }

    heap_free(cache);
    *audio_resource = NULL;
}
