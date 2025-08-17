#pragma once
#include "system.h"

struct test_component
{
    int number;
};

namespace Scene
{
    template <typename T>
    void please_work([[maybe_unused]] T &syscall, System::ECS::Query<test_component>& query)
    {
        for (auto& entry : query)
        {
            auto& comp = entry.get<test_component>();
            comp.number = 1;
            LOG_DEBUG("Info: WE WIN THIS!!!");
        }
    }

    struct Demo
    {
        constexpr static auto name = "Demo";
        // declare scene parameters
        constexpr static size_t MaxResource = 1000;
        using ComponentTuple = std::tuple<test_component>;

        using ComponentType = Utils::from_tuple_t<ComponentTuple>;
        using ResourceManager = System::ECS::ResourceManager<MaxResource, ComponentType>;
        using Syscall = System::ECS::Syscall<MaxResource, ComponentType>;

        // declare functions
        static void test();

        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall, please_work<Syscall>>;
    };
}
