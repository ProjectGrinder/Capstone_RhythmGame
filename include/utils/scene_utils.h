#pragma once
#include <memory>
#include <optional>
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

    // helper to make resource manager
    template<size_t MaxResource, typename ComponentTuple>
    using make_resource_manager_t = make_resource_manager<MaxResource, ComponentTuple>::type;

    template<size_t N, typename T>
    struct make_syscall;

    template<size_t N, typename... Components>
    struct make_syscall<N, std::tuple<Components...>>
    {
        using type = System::ECS::Syscall<N, Components...>;
    };

    // helper to make syscall
    template<size_t MaxResource, typename ComponentTuple>
    using make_syscall_t = make_syscall<MaxResource, ComponentTuple>::type;

    template<typename T>
    struct make_scene_variant;

    template<typename... Ts>
    struct make_scene_variant<std::tuple<Ts...>>
    {
        using type = std::variant<std::monostate, Ts...>;
    };

    // helper to make scene variant
    template<typename T>
    using make_scene_variant_t = make_scene_variant<T>::type;

    template<typename T>
    struct make_task_manager_variant;

    template<typename... Scenes>
    struct make_task_manager_variant<std::tuple<Scenes...>>
    {
        using type = std::variant<std::monostate, std::shared_ptr<typename Scenes::TaskManager>...>;
    };

    // helper to make task manager variant
    template<typename T>
    using make_task_manager_variant_t = make_task_manager_variant<T>::type;

    template<typename From, typename To, typename Resource>
    void convert_import_resource(typename From::ResourceManager &rm, typename To::ResourceManager &rm_converted)
    {
        auto source_pool = rm.template query_if_exists<Resource>();
        auto target_pool = rm_converted.template query<Resource>();
        (void) source_pool;
        (void) target_pool;
    }

    template<typename From, typename To, size_t... I>
    void convert_impl(typename From::ResourceManager &rm, typename To::ResourceManager &rm_converted, std::index_sequence<I...>)
    {
        using InComponentTuple = To::ComponentTuple;
        (convert_import_resource<From, To, std::tuple_element_t<I, InComponentTuple>>(rm, rm_converted), ...);
    }

    template<typename From, typename To>
    To::ResourceManager convert(typename From::ResourceManager &rm)
    {
        typename To::ResourceManager rm_converted;
        using InComponentTuple = To::ComponentTuple;

        convert_impl<From, To>(rm, rm_converted, std::make_index_sequence<std::tuple_size_v<InComponentTuple>>());

        return rm_converted;
    }

} // namespace Utils
