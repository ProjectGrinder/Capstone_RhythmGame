#include "scene.h"
#include "utils.h"
#include "system.h"

namespace Scene::Demo
{

    void please_work([[maybe_unused]]System::ECS::Syscall<1000, test_component> &S, System::ECS::Query<test_component>& query)
    {
        for (auto& entry : query)
        {
            auto& comp = entry.get<1>();
            comp.number = 1;
            LOG_DEBUG("Info: WE WIN THIS!!!");
        }
    }

    void Demo()
    {
        using DemoResource = System::ECS::ResourceManager<1000, test_component>;
        using DemoSyscall = System::ECS::Syscall<1000, test_component>;
        DemoResource resource;
        DemoSyscall syscall{resource};
        System::ECS::TaskManager<DemoResource, DemoSyscall, please_work> task_manager(resource, syscall);
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
