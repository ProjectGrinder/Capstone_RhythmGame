#include "scene.h"
#include "utils.h"
#include "system.h"

namespace Scene::Demo
{

    void please_work([[maybe_unused]]System::pid id, test_component& comp)
    {
        comp.number = 1;
        LOG_DEBUG("Info: WE WIN THIS!!!");
    }

    void Demo()
    {
        using DemoResource = System::ResourceManager<1000, test_component>;
        DemoResource resource;
        System::TaskManager<DemoResource, please_work> task_manager(resource);
        task_manager.run_all();
        LOG_DEBUG("Info: Demo call!");
        test();
    }

    void test()
    {
        test_component val;
        val.number = 1;
        LOG_DEBUG("Info: test call with val = {}", val.number);

        System::Game::change_scene(Demo2::Demo2);
    }
}
