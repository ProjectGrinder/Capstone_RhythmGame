#include "system/thread.h"
#define WIN32_LEAN_AND_MEAN

#if _DEBUG
    #define _CRTDBG_MAP_ALLOC
    #include <crtdbg.h>
#endif

#include <windows.h>

#include "utils/windows_utils.h"

#include "directx/directx_api.h"
#include "directx/dx11_adapter_api.h"
#include "scenes/compositor_api.h"
#include "scenes/intent_api.h"
#include "scenes/scenes_api.h"

#include "audio/os_audio.h"

#include "windows_functions.h"
#include "windows_types.h"

extern void assets_cleanup(void);
extern unsigned int intent_storage_get_current_frame();
extern void intent_storage_next_frame();

static SystemInfo system_info = {
        .window = {.width = 1920, .height = 1080},
        .monitor =
                {
                        .width = 0,
                        .height = 0,
                },
        .instance_handler = NULL,
        .window_handler = NULL,
        .display_type = DT_WINDOW,
        .is_running = 0,
        .precision = 1,
        .perf_frequency = {0},
        .delta_time = 0,
        .vertex_queue = NULL,
        .rendering_queue = NULL,
        .scene_manager = NULL,
        .intent_storage = NULL,
        .compositor = NULL,
        .dx11_adapter = NULL,
        .directx = NULL,
        .audio = NULL,
};

static ThreadHandle g_render_thread = NULL;
static EventHandle g_render_event = NULL;
static atomic_size_t g_ready_frame_idx = (size_t) -1;

Window get_window_size(void)
{
    return (system_info.window);
}

HWND get_window_handler(void)
{
    return (system_info.window_handler);
}

LARGE_INTEGER get_perf_frequency(void)
{
    return (system_info.perf_frequency);
}

void *get_scene_manager(void)
{
    return (system_info.scene_manager);
}

void *get_intent_storage(void)
{
    return (system_info.intent_storage);
}

void *get_compositor(void)
{
    return (system_info.compositor);
}

void *get_dx11_adapter()
{
    return (system_info.dx11_adapter);
}

void *get_audio_api(void)
{
    return (system_info.audio);
}

long double get_delta_time(void)
{
    return (system_info.delta_time);
}

HRESULT directx_init(_In_ DirectXHandler *directx_api)
{
    const DirectXConfig config = {
            .height = system_info.window.height,
            .width = system_info.window.width,
            .window_handler = system_info.window_handler,
            .is_window = system_info.display_type == DT_WINDOW,
    };
    return (directx_device_init(directx_api, &config));
}

__forceinline void render_present(_In_ DirectXHandler *directx_api)
{
    directx_device_present(directx_api);
}

__forceinline void directx_cleanup(_In_ DirectXHandler *directx_api)
{
    directx_device_cleanup(directx_api);
}

unsigned long render_thread_loop(void *args)
{
    UNREFERENCED_PARAMETER(args);
    LOG_INFO("Render Thread Started.");

    LARGE_INTEGER start, end;

    while (system_info.is_running)
    {
        event_wait(g_render_event);

        if (!system_info.is_running)
            break;

        int frame_to_draw = (int) ATOMIC_LOAD(&g_ready_frame_idx);
        ATOMIC_STORE(&g_ready_frame_idx, (size_t) -1);

        if (frame_to_draw != -1)
        {
            QueryPerformanceCounter(&start);

            compositor_compose(&system_info.intent_storage, &system_info.compositor, frame_to_draw);
            dx11_adapter_convert(&system_info.dx11_adapter, &system_info.directx, &system_info.compositor);
            dx11_adapter_render(&system_info.dx11_adapter, &system_info.directx);

            render_present(&system_info.directx);

            QueryPerformanceCounter(&end);
        }
    }

    LOG_INFO("Render Thread Exiting.");
    return 0;
}

int real_main()
{
#if _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    DWORD error = 0;
    MSG msg = {0};
    system_info.is_running = 1;

    log_init();
    sleep_init(&system_info.perf_frequency);

    LOG_INFO("Starting...");
    error = create_window(&system_info);
    if (error != ERROR_SUCCESS)
    {
        LOG_ERROR("create_window failed, Code 0x%08lx", error);
        goto exit;
    }

    error = directx_init(&system_info.directx);
    if (error != ERROR_SUCCESS)
    {
        LOG_ERROR("directx_init failed, Code 0x%08lx", error);
        goto exit;
    }

    error = intent_storage_init(&system_info.intent_storage);
    if (error != ERROR_SUCCESS)
    {
        LOG_ERROR("intent_storage_init failed, Code 0x%08lx", error);
        goto exit;
    }

    error = compositor_init(&system_info.compositor);
    if (error != ERROR_SUCCESS)
    {
        LOG_ERROR("compositor_init failed, Code 0x%08lx", error);
        goto exit;
    }

    error = dx11_adapter_init(&system_info.dx11_adapter, &system_info.directx);
    if (error != ERROR_SUCCESS)
    {
        LOG_ERROR("dx11_adapter_init failed, Code 0x%08lx", error);
        goto exit;
    }

    error = scene_manager_init(&system_info.scene_manager);
    if (error != ERROR_SUCCESS)
    {
        LOG_ERROR("scene_manager_init failed, Code 0x%08lx", error);
        goto exit;
    }

    error = audio_init(&system_info.audio);
    if (error != ERROR_SUCCESS)
    {
        LOG_ERROR("audio_init failed, Code 0x%08lx", error);
        goto exit;
    }

    LARGE_INTEGER start, end;
    long double input, scene;

    g_render_event = event_create();
    g_render_thread = thread_create(render_thread_loop, NULL);

    while (system_info.is_running)
    {
        QueryPerformanceCounter(&start);
        process_system_message(&system_info, &msg);
        QueryPerformanceCounter(&end);
        input = ((long double) (end.QuadPart - start.QuadPart) * 1000L) /
                (long double) system_info.perf_frequency.QuadPart;

        scene_manager_update(&system_info.scene_manager);
        QueryPerformanceCounter(&end);
        scene = ((long double) (end.QuadPart - start.QuadPart) * 1000L) /
                (long double) system_info.perf_frequency.QuadPart;

        unsigned int finished_frame = intent_storage_get_current_frame();
        ATOMIC_STORE(&g_ready_frame_idx, (size_t) finished_frame);

        event_signal(g_render_event);
        intent_storage_next_frame();

        QueryPerformanceCounter(&end);
        system_info.delta_time = ((long double) (end.QuadPart - start.QuadPart) * 1000L) /
                                 (long double) system_info.perf_frequency.QuadPart;

        LOG_INFO(
                "Game Thread: %d us [ Input: %d us, Scene: %d us ]",
                (int) (system_info.delta_time * 1000.0L),
                (int) (input * 1000.0L),
                (int) ((scene - input) * 1000.0L));


        sleep(max(system_info.precision - (LONGLONG) system_info.delta_time, 0));
        system_info.delta_time = max(system_info.delta_time, system_info.precision);
    }

exit:
    LOG_INFO("Cleaning up");

    if (g_render_event != NULL)
    {
        event_signal(g_render_event);
    }
    if (g_render_thread != NULL)
    {
        thread_join(g_render_thread);
    }
    if (g_render_event != NULL)
    {
        event_destroy(g_render_event);
    }

    scene_manager_cleanup(&system_info.scene_manager);
    intent_storage_cleanup(&system_info.intent_storage);
    compositor_cleanup(&system_info.compositor);
    dx11_adapter_cleanup(&system_info.dx11_adapter);
    directx_cleanup(&system_info.directx);
    audio_handler_cleanup(&system_info.audio);
    assets_cleanup();
    log_cleanup();

#if _DEBUG
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
#endif
    return ((int) error);
}

/*
 *  Provided 2 main functions for 2 entry points on Windows
 *  1. Console
 *  2. Windows Application
 */
int main(int argc, char **argv)
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);
    return (real_main());
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ INT nShowCmd)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nShowCmd);

    system_info.instance_handler = hInstance;

    return (main(0, NULL));
}
