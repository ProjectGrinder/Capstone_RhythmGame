#pragma once
#include <bit>
#include <bitset>
#include <tuple>
#include <type_traits>

#include "ecs_types.h"


namespace System::ECS
{

    namespace Utils
    {
        template<auto Target, auto... Pack>
        constexpr size_t get_task_index()
        {
            size_t index = 0;
            (void) ((Pack == Target ? true : (++index, false)) || ...);
            return index;
        }

        template<typename T>
        struct extract_all_queries;

        template<typename Ret, typename Sys, typename... Queries>
        struct extract_all_queries<Ret (*)(Sys &, Queries &...)>
        {
            using type = std::tuple<std::remove_cvref_t<Queries>...>;
        };

    } // namespace Utils

    template<typename ResourceManager, SyscallType Syscall, auto... Tasks>
        requires(TaskConcept<decltype(Tasks)> && ...)
    class TaskManager
    {
        ResourceManager _resource_manager;
        Syscall _syscall;

        static constexpr size_t MaxResource = ResourceManager::max_resource_v;

        template<typename T>
        struct extract_query;

        template<typename Sys, typename Q>
        struct extract_query<void (*)(Sys &, Q &)>
        {
            using type = std::remove_cvref_t<Q>;
        };

        using QueryCache_t = std::tuple<typename Utils::extract_all_queries<decltype(Tasks)>::type...>;
        QueryCache_t _query_cache;

        uint64_t extract_chunk_from_bitset(const std::bitset<MaxResource> &b, size_t chunk_idx)
        {
            return reinterpret_cast<const uint64_t *>(&b)[chunk_idx];
        }

        template<typename QueryObject, std::size_t... I>
        auto _populate_query(QueryObject &query, std::index_sequence<I...>)
        {
            using ComponentTuple = QueryObject::ComponentTuple;
            // using First = std::remove_reference_t<std::tuple_element_t<0, ComponentTuple>>;
            // auto &main_pool = _resource_manager.template query<First>();
            query.clear();

            auto cached_pools = std::tie(
                    _resource_manager
                            .template query<std::remove_reference_t<std::tuple_element_t<I, ComponentTuple>>>()...);

            auto mask = _resource_manager.get_bitset();
            ((mask &=
              _resource_manager.template query<std::remove_reference_t<std::tuple_element_t<I, ComponentTuple>>>()
                      .get_bitset()),
             ...);

            /*
            for (size_t id = 0; id < MaxResource; ++id)
            {
                if (mask.test(id))
                {
                    query.add(id, std::get<I>(cached_pools).get(id)...);
                }
            }
            */
            // std::for_each(
            //         main_pool.begin(),
            //         main_pool.end(),
            //         [&cached_pools, &query](const auto &pair)
            //         {
            //             if (auto id = pair.first; (... && std::get<I>(cached_pools).has(id)))
            //             {
            //                 query.add(id, std::get<I>(cached_pools).get(id)...);
            //             }
            //         });
            constexpr size_t bits_per_chunk = 64;
            for (size_t chunk_idx = 0; chunk_idx < MaxResource / bits_per_chunk; ++chunk_idx)
            {
                // Get the raw 64-bit word (this is where the speed is)
                uint64_t chunk = extract_chunk_from_bitset(mask, chunk_idx);

                while (chunk != 0)
                {
                    int tz = std::countr_zero(chunk);
                    size_t id = (chunk_idx * bits_per_chunk) + tz;

                    query.add(static_cast<pid>(id), std::get<I>(cached_pools).get(id)...);

                    chunk &= (chunk - 1);
                }
            }
            return (query);
        }

        template<typename QObj>
        void _prepare_query(QObj &query)
        {
            uint64_t world_v = _resource_manager.get_version();
            if (!query.is_stale(world_v)) [[likely]]
                return;

            using ComponentTuple = typename QObj::ComponentTuple;
            _populate_query(query, std::make_index_sequence<std::tuple_size_v<ComponentTuple>>{});
            query.sync_version(world_v);
        }

        template<size_t TaskIdx>
        void _run_at_index()
        {
            constexpr auto Task = std::get<TaskIdx>(std::forward_as_tuple(Tasks...));
            auto &query_bundle = std::get<TaskIdx>(_query_cache);

            std::apply([this](auto &...queries) { (this->_prepare_query(queries), ...); }, query_bundle);
            std::apply([this, Task](auto &...queries) { Task(_syscall, queries...); }, query_bundle);
        }

    public:
        TaskManager() : _syscall(_resource_manager)
        {}

        TaskManager(const TaskManager &) = delete;
        TaskManager &operator=(const TaskManager &) = delete;
        TaskManager(TaskManager &&) = delete;
        TaskManager &operator=(TaskManager &&) = delete;

        inline void run_all()
        {
            _syscall.exec();
            auto runner = [this]<std::size_t... Is>(std::index_sequence<Is...>)
            { (this->template _run_at_index<Is>(), ...); };
            runner(std::make_index_sequence<sizeof...(Tasks)>{});
        }

        template<typename Component>
        void add_component(pid id, Component &&component)
        {
            using Stored = std::remove_cvref_t<Component>;
            _syscall.template add_component<Stored>(id, std::forward<Component>(component));
        }

        template<typename Component>
        void remove_component(pid id)
        {
            _syscall.template remove_component<Component>(id);
        }

        template<typename... Components>
        pid create_entity(Components &&...components)
        {
            return (_syscall.template create_entity<std::remove_cvref_t<Components>...>(
                    std::forward<Components>(components)...));
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
