#pragma once
#include <execution>
#include <tuple>
#include <type_traits>

#include "ecs_types.h"

namespace System::ECS
{
    template<typename ResourceManager, SyscallType Syscall, auto... Tasks>
    class TaskManager
    {
        ResourceManager& _resource_manager;
        Syscall& _syscall;

        template<typename...>
        using void_t = void;

        template<typename Func>
        struct function_traits;

        template<typename Ret, typename Entity, typename First, typename... Rest>
        struct function_traits<Ret(Entity, First, Rest...)>
        {
            static_assert(!SyscallType<std::remove_reference_t<First>>,
             "Regular function trait matched with Syscall parameter - use specialized version");
            using return_type = Ret;
            using component_tuple = std::tuple<First, Rest...>;
            static constexpr bool has_syscall = false;
        };

        template<typename Ret, typename Entity, SyscallType S, typename... Rest>
        struct function_traits<Ret(Entity, S&, Rest...)>
        {
            using return_type = Ret;
            using component_tuple = std::tuple<Rest...>;
            static constexpr bool has_syscall = true;
        };

        template<typename Ret, typename Entity, typename First, typename... Rest>
        struct function_traits<Ret(*)(Entity, First, Rest...)>
            : function_traits<Ret(Entity, First, Rest...)> {};


        template<typename Func>
        struct function_traits
            : function_traits<decltype(&Func::operator())>{};

        template<typename Func, std::size_t... I>
        inline void _run_impl(Func system, std::index_sequence<I...>) const noexcept
        {
            using FnTraits = function_traits<Func>;
            using ComponentTuple = typename FnTraits::component_tuple;

            using First = std::remove_reference_t<std::tuple_element_t<0, ComponentTuple>>;
            auto& main_pool = _resource_manager.template query<First>();
            auto cached_pools = std::tie(
                _resource_manager.template query<std::remove_reference_t<std::tuple_element_t<I, ComponentTuple>>>()...
            );

            if constexpr (FnTraits::has_syscall)
            {
                std::for_each(std::execution::par, main_pool.begin(), main_pool.end(),
                    [this, cached_pools, system](auto pair)
                    {
                        auto id = pair.first;
                        if ((... && std::get<I>(cached_pools).has(id)))
                        {
                            system(id, _syscall, std::get<I>(cached_pools).get(id)...);
                        }
                    });
            } else
            {
                std::for_each(std::execution::par, main_pool.begin(), main_pool.end(),
                    [cached_pools, system](auto pair)
                    {
                        auto id = pair.first;
                        if ((... && std::get<I>(cached_pools).has(id)))
                        {
                            system(id, std::get<I>(cached_pools).get(id)...);
                        }
                    });
            }
        }

        template<typename Func>
        inline void _run(Func system) const noexcept
        {
            using FnTraits = function_traits<Func>;
            using ComponentTuple = typename FnTraits::component_tuple;
            constexpr std::size_t N = std::tuple_size_v<ComponentTuple>;
            _run_impl(system, std::make_index_sequence<N>{});
        }

    public:
        explicit TaskManager(ResourceManager& reg, Syscall& syscall)
        : _resource_manager(reg), _syscall(syscall) {}

        inline void run_all() const
        {
            (_run(Tasks), ...);
            _syscall.exec();
        }
    };
}