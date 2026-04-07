#pragma once
#include <memory>
#include <variant>
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

    template<typename T>
    struct make_scene_variant;

    template<typename... Ts>
    struct make_scene_variant<std::tuple<Ts...>>
    {
        using type = std::variant<std::monostate, Ts...>;
    };

    template<typename T>
    using make_scene_variant_t = typename make_scene_variant<T>::type;

    template<typename T>
    struct make_task_manager_variant;

    template<typename... Scenes>
    struct make_task_manager_variant<std::tuple<Scenes...>>
    {
        using type = std::variant<std::monostate, std::shared_ptr<typename Scenes::TaskManager>...>;
    };

    template<typename T>
    using make_task_manager_variant_t = typename make_task_manager_variant<T>::type;

} // namespace Utils
