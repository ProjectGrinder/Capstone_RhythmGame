#include "scene.h"
#include "system.h"
#include "utils.h"

namespace Scene::Demo
{

    void please_work([[maybe_unused]]System::pid id, test_component& comp)
    {
        comp.number = 1;
        LOG_DEBUG("Info: WE WIN THIS!!!");
    }

    System::Render2D render_test_component(test_component& comp)
    {
    }

    System::Render3D render_test_component2(test_component& comp)
    {
    }

    struct Demo
    {
        using Resource = System::ResourceManager<1000, test_component>;
        using Syscall = System::Syscall<1000, test_component>;
        using Manager = System::TaskManager<Resource, Syscall, please_work>;
        using Renderer = System::Renderer<Resource, render_test_component, render_test_component2>;
    };
}
