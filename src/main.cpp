#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <Windows.h>

#include "scene.h"
#include "system.h"

struct TestA {};

int WINAPI WinMain(
    _In_        [[maybe_unused]]HINSTANCE   hInstance,
    _In_opt_    [[maybe_unused]]HINSTANCE   hPrevInstance,
    _In_        [[maybe_unused]]char        *lpCmdLine,
    _In_        [[maybe_unused]]int         nShowCmd
)
{
#if _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    uint32_t error = ERROR_SUCCESS;

    LOG_DEBUG("Info: Initialization...");
    using System::OS, System::Game, Scene::SceneConfig::starting_scene;

    Game::change_scene(starting_scene);

    LOG_DEBUG("Info: Starting...");
    if (error == ERROR_SUCCESS)
    {
        error = OS::run();
    }

    LOG_DEBUG("Info: Cleaning up...");
    OS::clean_up();

#if _DEBUG
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG); 
    _CrtDumpMemoryLeaks();
#endif
    return((int)error);
}