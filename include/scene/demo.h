#pragma once
#include "system.h"
#include "game.h"

namespace Scene
{

    struct test_component
    {
        int number;
    };

    template <typename T>
    void please_work([[maybe_unused]] T &syscall, System::ECS::Query<test_component>& query)
    {
        for (auto& entry : query)
        {
            auto& comp = entry.get<test_component>();
            comp.number += 1;
            if (comp.number > 10)
            {
                comp.number = 0;
            }
        }
    }

    // template <typename T>
    // void please_work2([[maybe_unused]] T &syscall, System::ECS::Query<System::Renderer::Render2D>& query)
    // {
    //     for (auto& entry : query)
    //     {
    //         entry.get<System::Renderer::Render2D>();
    //     }
    // }

    struct Demo
    {

        static Demo instance();

        constexpr static auto name = "Demo";
        // declare scene parameters
        constexpr static size_t MaxResource = 1000;
        using ComponentTuple = std::tuple<test_component>;
        using ResourceManager = System::ECS::ResourceManager<MaxResource, test_component>;
        using Syscall = System::ECS::Syscall<MaxResource, test_component>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall, please_work<Syscall>>;

        // declare functions
        static void test();
        static TaskManager Init();
        static std::vector<ComponentTuple> Exit();
    };
}
