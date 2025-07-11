#pragma once

#include <tuple>
#include <utility>
#include <vector>
#include "ecs_types.h"

namespace System::ECS
{

    template<typename... Components>
    class Query
    {
    public:
        using ComponentTuple = std::tuple<Components...>;
        using StoredTuple = std::tuple<std::reference_wrapper<Components>...>;

        struct QueryEntry {
            pid id;
            StoredTuple components;

            template<size_t I = 0>
            decltype(auto) get() {
                if constexpr (I == 0) {
                    return id;
                } else {
                    return std::get<I-1>(components).get();
                }
            }
        };

    private:
        mutable std::vector<QueryEntry> _entries;

    public:
        template<typename... Comps>
            requires (std::is_convertible_v<Comps&, Components&> && ...)
        void add(pid id, Comps&... components) const {
            _entries.emplace_back(QueryEntry{id, StoredTuple{std::ref(components)...}});
        }

        auto begin() { return _entries.begin(); }
        auto end() { return _entries.end(); }
        auto begin() const { return _entries.begin(); }
        auto end() const { return _entries.end(); }

        void clear() { _entries.clear(); }
    };


}