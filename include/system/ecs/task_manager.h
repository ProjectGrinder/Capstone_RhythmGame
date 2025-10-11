#pragma once
#include <algorithm>
#include <execution>
#include <tuple>
#include <type_traits>

#include "ecs_types.h"

namespace System::ECS
{
    template<typename ResourceManager, SyscallType Syscall, auto... Tasks>
        requires(TaskConcept<decltype(Tasks)> && ...)
    class TaskManager
    {
        ResourceManager _resource_manager;
        Syscall _syscall;

        template<QueryType QueryRef, std::size_t... I>
        auto _make_query_impl(std::index_sequence<I...>)
        {
            using QueryObject = std::remove_reference_t<QueryRef>;
            using ComponentTuple = typename QueryObject::ComponentTuple;
            using First = std::remove_reference_t<std::tuple_element_t<0, ComponentTuple>>;
            auto &main_pool = _resource_manager.template query<First>();
            auto cached_pools = std::tie(
                    _resource_manager
                            .template query<std::remove_reference_t<std::tuple_element_t<I, ComponentTuple>>>()...);
            QueryObject query;

            std::for_each(
                    std::execution::par,
                    main_pool.begin(),
                    main_pool.end(),
                    [&cached_pools, &query](const auto &pair)
                    {
                        if (auto id = pair.first; (... && std::get<I>(cached_pools).has(id)))
                        {
                            query.add(id, std::get<I>(cached_pools).get(id)...);
                        }
                    });
            return (query);
        }

        template<QueryType QueryRef>
        auto _make_query()
        {
            constexpr std::size_t N = std::tuple_size_v<typename std::remove_reference_t<QueryRef>::ComponentTuple>;
            return (_make_query_impl<QueryRef>(std::make_index_sequence<N>{}));
        }

        template<typename ArgsTuple, std::size_t... I>
        auto _run_impl(std::index_sequence<I...>)
        {
            return std::make_tuple(_make_query<std::tuple_element_t<I + 1, ArgsTuple>>()...);
        }

        template<auto Task>
        void _run()
        {
            using TaskType = decltype(Task);
            using ArgsTuple = typename function_traits<TaskType>::args_tuple;
            constexpr std::size_t N = std::tuple_size_v<ArgsTuple>;

            auto queries = _run_impl<ArgsTuple>(std::make_index_sequence<N - 1>{});

            std::apply(
                    [&syscall = _syscall]<typename... QueryTypes>(QueryTypes &&...q)
                    { Task(syscall, std::forward<QueryTypes>(q)...); },
                    queries);
        }

    public:
        TaskManager()
        {
            _resource_manager = ResourceManager();
            _syscall = Syscall();
        }

        inline void run_all()
        {
            (_run<Tasks>(), ...);
            _syscall.exec(_resource_manager);
        }

        template <typename Component>
        void add_component(pid id, Component &&component)
        {
            _syscall.template add_component<Component>(id, std::forward<Component>(component));
        }

        template<typename Component>
        void remove_component(pid id)
        {
            _syscall.template remove_component<Component>(id);
        }

        template<typename... Components>
        pid create_entity(Components &&...components)
        {
            return _syscall.template create_entity<Components...>(_resource_manager, std::forward<Components>(components)...);
        }

        void remove_entity(const pid id)
        {
            _syscall.remove_entity(id);
        }

        auto get_rm()
        {
            return (&_resource_manager);
        }
    };
} // namespace System::ECS
