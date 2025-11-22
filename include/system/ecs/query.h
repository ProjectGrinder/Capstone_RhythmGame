#pragma once

#include <tuple>
#include <utility>
#include <vector>
#include "ecs_types.h"

namespace System::ECS::Utils
{
    template<typename T, typename Tuple>
    constexpr std::size_t tuple_find_v = []()
    {
        constexpr auto helper = []<std::size_t... I>(std::index_sequence<I...>)
        { return ((std::is_same_v<T, std::tuple_element_t<I, Tuple>> ? I : 0) + ...); };
        return (helper(std::make_index_sequence<std::tuple_size_v<Tuple>>{}));
    }();

} // namespace System::ECS::Utils

namespace System::ECS
{

    template<typename... Components>
    class Query
    {
    public:
        using ComponentTuple = std::tuple<Components...>;

        struct StoredTuple
        {
            std::tuple<std::reference_wrapper<Components>...> components{};

            explicit StoredTuple(Components&... comps) : components(std::ref(comps)...) {}  // Add constructor

            template<typename Component>
            decltype(auto) get()
            {
                return (std::get<Utils::tuple_find_v<Component, std::tuple<Components...>>>(components).get());
            }
        };

        struct QueryEntry
        {
            pid id{};
            StoredTuple components;

            template<typename Component>
            decltype(auto) get()
            {
                if constexpr (std::is_same_v<Component, pid>)
                {
                    return (id);
                }
                else
                {
                    return components.template get<Component>();
                }
            }
        };

    private:
        std::vector<QueryEntry> _entries;

    public:
        template<typename... Comps>
            requires(std::is_convertible_v<Comps &, Components &> && ...)
        void add(pid id, Comps &...components)
        {
            _entries.emplace_back(QueryEntry{id, StoredTuple{std::ref(components)...}});
        }

        QueryEntry front()
        {
            return (_entries.front());
        }

        auto begin()
        {
            return (_entries.begin());
        }
        auto end()
        {
            return (_entries.end());
        }
        auto begin() const
        {
            return (_entries.begin());
        }
        auto end() const
        {
            return (_entries.end());
        }
    };

} // namespace System::ECS
