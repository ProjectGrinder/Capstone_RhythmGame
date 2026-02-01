#define WIN32_LEAN_AND_MEAN

#if _DEBUG
    #define _CRTDBG_MAP_ALLOC
    #include <crtdbg.h>
#endif

#include <windows.h>

#include "utils/windows_utils.h"

#include "directx/directx_api.h"
#include "scenes/intent_api.h"
#include "scenes/scenes_api.h"

#include "windows_functions.h"
#include "windows_types.h"

extern void assets_cleanup();

static SystemInfo system_info = {
        .window = {.width = 1280, .height = 720},
        .monitor =
                {
                        .width = 0,
                        .height = 0,
                },
        .instance_handler = NULL,
        .window_handler = NULL,
        .display_type = DT_WINDOW,
        .is_running = 0,
        .precision = 15,
        .vertex_queue = NULL,
        .rendering_queue = NULL,
        .scene_manager = NULL,
        .intent_storage = NULL,
        .directx = NULL,
};

HWND get_window_handler()
{
    return (system_info.window_handler);
}

void *get_scene_manager()
{
    return (system_info.scene_manager);
}

void *get_intent_storage()
{
    return (system_info.intent_storage);
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

__forceinline void directx_clean_up(_In_ DirectxHandler *directx_api)
{
    directx_device_clean_up(directx_api);
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
    sleep_init();

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

    error = scene_manager_init(&system_info.scene_manager);
    if (error != ERROR_SUCCESS)
    {
        LOG_ERROR("scene_manager_init failed, Code 0x%08lx", error);
        goto exit;
    }

    error = intent_storage_init(&system_info.intent_storage);
    if (error != ERROR_SUCCESS)
    {
        LOG_ERROR("intent_storage_init failed, Code 0x%08lx", error);
        goto exit;
    }

    while (system_info.is_running)
    {
        process_system_message(&system_info, &msg);

        scene_manager_update(&system_info.scene_manager);

        sleep(system_info.precision);
    }

exit:
    LOG_INFO("Cleaning up");
    scene_manager_cleanup(&system_info.scene_manager);
    directx_clean_up(&system_info.directx);
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
