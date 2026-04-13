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


    template<typename T, typename Tuple>
    constexpr std::size_t tuple_find_v = []()
    {
        constexpr auto helper = []<std::size_t... I>(std::index_sequence<I...>)
        {
            auto result = static_cast<std::size_t>(-1);

            ((std::is_same_v<T, std::tuple_element_t<I, Tuple>> ? result = I : 0), ...);

            return result;
        };

        return (helper(std::make_index_sequence<std::tuple_size_v<Tuple>>{}));
    }();

    template<typename T, typename Tuple>
    inline constexpr bool tuple_contains_v = (tuple_find_v<T, Tuple> != static_cast<std::size_t>(-1));

    template<typename From, typename To>
    To::ComponentTuple map(typename From::ComponentTuple &from)
    {
        using FromTuple = From::ComponentTuple;
        using ToTuple = To::ComponentTuple;

        return (
            []<std::size_t... Is>(FromTuple &src, std::index_sequence<Is...>)
            {
                return (ToTuple{(
                    []<typename Elem>(FromTuple &tuple) -> Elem
                            {
                                if constexpr (tuple_contains_v<Elem, FromTuple>)
                                {
                                    return (std::get<tuple_find_v<Elem, FromTuple>>(tuple));
                                }
                                else
                                {
                                   return (Elem{});
                                }
                           }.template operator()<std::tuple_element_t<Is, ToTuple>>(src))...});
            }(from, std::make_index_sequence<std::tuple_size_v<ToTuple>>{}));
    }

    template<typename From, typename To>
    std::vector<typename To::ComponentTuple> map(std::vector<typename From::ComponentTuple> &from)
    {
        std::vector<typename To::ComponentTuple> out;
        out.reserve(from.size());
        for (auto &component_tuple: from)
        {
            out.emplace_back(map<From, To>(component_tuple));
        }
        return (out);
    }

} // namespace Utils
