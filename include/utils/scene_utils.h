#pragma once
#include "system/ecs/ecs_types.h"

namespace Utils
{
    template<size_t N, typename T>
    struct make_resource_manager;

    template<size_t N, typename... Components>
    struct make_resource_manager<N, std::tuple<Components...>>
    {
        using type = System::ECS::ResourceManager<N, Components...>;
    };

    template<size_t MaxResource, typename ComponentTuple>
    using make_resource_manager_t = typename make_resource_manager<MaxResource, ComponentTuple>::type;

    template<size_t N, typename T>
    struct make_syscall;

    template<size_t N, typename... Components>
    struct make_syscall<N, std::tuple<Components...>>
    {
        using type = System::ECS::Syscall<N, Components...>;
    };

    template<size_t MaxResource, typename ComponentTuple>
    using make_syscall_t = typename make_syscall<MaxResource, ComponentTuple>::type;
} // namespace Utils
