#pragma once
#include <tuple>
#include <type_traits>

#include "ecs_types.h"

namespace System::ECS
{
    template<typename ResourceManager, SyscallType Syscall, auto... Tasks>
        requires (TaskConcept<decltype(Tasks)> && ...)
    class TaskManager
    {
        ResourceManager& _resource_manager;
        Syscall& _syscall;

        template<QueryType QueryRef, std::size_t... I>
        auto _make_query_impl(std::index_sequence<I...>) const
        {
            using QueryObject = std::remove_reference_t<QueryRef>;
            using ComponentTuple = typename QueryObject::ComponentTuple;
            using First = std::remove_reference_t<std::tuple_element_t<0, ComponentTuple>>;
            auto& main_pool = _resource_manager.template query<First>();
            auto cached_pools = std::tie
            (
                _resource_manager.template query<std::remove_reference_t<std::tuple_element_t<I, ComponentTuple>>>()...
            );
            QueryObject query;
            for (const auto& pair : main_pool)
            {
                auto id = pair.first;
                if ((... && std::get<I>(cached_pools).has(id)))
                {
                    query.add(id, std::get<I>(cached_pools).get(id)...);
                }
            }
            return (query);
        }

        template<QueryType QueryRef>
        auto _make_query() const
        {
            constexpr std::size_t N = std::tuple_size_v<typename std::remove_reference_t<QueryRef>::ComponentTuple>;
            return (_make_query_impl<QueryRef>(std::make_index_sequence<N>{}));
        }

        template<auto Task>
        void _run() const
        {
            using TaskType = decltype(Task);
            using args_tuple = typename function_traits<TaskType>::args_tuple;
            constexpr std::size_t N = std::tuple_size_v<args_tuple>;
            auto call_with_args = []<std::size_t... I>(const auto* self, std::index_sequence<I...>)
            {
                auto queries = std::make_tuple(self->template _make_query<std::tuple_element_t<I + 1, args_tuple>>()...);
                Task(self->_syscall, std::get<I>(queries)...);
            };
            call_with_args(this, std::make_index_sequence<N - 1>{});
        }

    public:
        explicit TaskManager(ResourceManager& reg, Syscall& syscall)
        : _resource_manager(reg), _syscall(syscall) {}

        inline void run_all() const
        {
            (_run<Tasks>(), ...);
            _syscall.exec();
        }
    };
}