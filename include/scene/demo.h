#pragma once
#include "system.h"

struct test_component
{
    int number;
};
namespace Scene
{
    struct Demo
    {
        // declare scene parameters
        constexpr static size_t MaxResource = 1000;
        using ComponentTuple = std::tuple<test_component>;

        using ComponentType = Utils::from_tuple_t<ComponentTuple>;
        using ResourceManager = System::ECS::ResourceManager<MaxResource, ComponentType>;
        using Syscall = System::ECS::Syscall<MaxResource, ComponentType>;

        // declare functions
        static void test();
        static void please_work(Syscall &syscall, System::ECS::Query<test_component>& query);

        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall, please_work>;
    };
}
