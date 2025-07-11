#include "scene.h"
#include "system.h"
#include "utils.h"

namespace Scene::Demo2
{

    struct demo2
    {
        int a;
        int b;
    };

    struct demo3
    {
        int a;
        int b;
    };

    void demo2_system([[maybe_unused]] System::ECS::Syscall<1000, demo2, demo3> &S, [[maybe_unused]] System::ECS::Query<demo2, demo3> &query)
    {
        for ([[maybe_unused]] auto& entry : query)
        {

        }
    }

    void Demo2()
    {
        using DemoResource = System::ECS::ResourceManager<1000, demo2, demo3>;
        using DemoSyscall = System::ECS::Syscall<1000, demo2, demo3>;
        DemoResource resource;
        DemoSyscall syscall{resource};
        System::ECS::TaskManager<DemoResource, DemoSyscall, demo2_system> task_manager(resource, syscall);
        LOG_DEBUG("Info: Demo2 called!");
        test2();
    }

    void test2()
    {
        LOG_DEBUG("Info: test2 called!");
    }
} // namespace Scene::Demo2
