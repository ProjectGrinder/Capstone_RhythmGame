#include "scene.h"
#include "utils.h"
#include "system.h"

namespace Scene::Demo
{
    void Demo()
    {
        LOG_DEBUG("Demo call!");
        test();
    }

    void test()
    {
        test_component val;
        val.number = 1;
        LOG_DEBUG("test call: {}", val.number);

        System::Game::change_scene(Demo2::Demo2);
    }
}

