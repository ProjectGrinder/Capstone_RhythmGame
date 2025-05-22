#include <Windows.h>

#include "scene.h"
#include "system.h"

int WINAPI WinMain(
    _In_        [[maybe_unused]]HINSTANCE   hInstance,
    _In_opt_    [[maybe_unused]]HINSTANCE   hPrevInstance,
    _In_        [[maybe_unused]]char        *lpCmdLine,
    _In_        [[maybe_unused]]int         nShowCmd
)
{
    uint32_t error = ERROR_SUCCESS;

    using System::OS, System::Game, Scene::SceneConfig::starting_scene;
    Game::change_scene(starting_scene);

    /* Initialize */
    if (error == ERROR_SUCCESS)
    {
        /* Starting OS Process */
        error = OS::run();
    }

    /* Cleanup (Make sure this were run) */
    OS::clean_up();

    return((int)error);
}