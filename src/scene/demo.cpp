#include "scene.h"
#include "system.h"

namespace Scene::Demo
{
    struct test_component
    {
        int number;
    };

    using DemoManager = System::ResourceManager<1000, test_component>;
    using DemoSyscall = System::Syscall<1000, test_component>;

    void init_test([[maybe_unused]]DemoSyscall &syscall);
    void update_test([[maybe_unused]]System::pid id, test_component& comp);
    void exit_test([[maybe_unused]]test_component& comp);
    System::Render2D render_test_component(test_component& comp);
    System::Render3D render_test_component2(test_component& comp);

    struct Demo
    {
        using ResourceManager = DemoManager;
        using Syscall = DemoSyscall;
        using Init = std::tuple<decltype(init_test)>;
        using Update = std::tuple<decltype(update_test)>;
        using Render = std::tuple<decltype(render_test_component), decltype(render_test_component2)>;
        using Exit = std::tuple<decltype(exit_test)>;
    };
}
