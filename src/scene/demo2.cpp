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

    void demo2_system([[maybe_unused]]System::pid id, [[maybe_unused]]demo2 &demo, [[maybe_unused]]demo3 &val)
    {
    }

    void Demo2()
    {
        using DemoResource = System::ResourceManager<1000, demo2, demo3>;
        DemoResource resource;
        System::TaskManager<DemoResource, demo2_system> task_manager(resource);
        LOG_DEBUG("Info: Demo2 called!");
        test2();
    }

    void test2()
    {
        LOG_DEBUG("Info: test2 called!");
    }
}
