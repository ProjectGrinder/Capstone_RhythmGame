#pragma once

#include <iterator>
#include <tuple>
#include <utility>
#include <vector>
#include "ecs_types.h"

namespace System::ECS
{

    template<typename... Components>
    struct Query
    {
        std::tuple<std::vector<pid>, std::vector<Components &>...> data;
        std::size_t size() const
        {
            return std::get<0>(data).size();
        }

        struct Iterator
        {
            using difference_type = std::ptrdiff_t;
            using value_type = std::tuple<pid, Components &...>;
            using pointer = value_type *;
            using reference = value_type &;
            using iterator_category = std::forward_iterator_tag;

            std::size_t idx;
            Query *query;
            mutable value_type cache; // mutable so operator* can update it

            Iterator(Query *q, const std::size_t i) : idx(i), query(q)
            {}

            reference operator*() const
            {
                auto &pid_vector = std::get<0>(query->data);
                auto get_ref = [this](auto &vec) -> auto & { return vec[this->idx]; };
                cache = std::tuple_cat(
                        std::make_tuple(pid_vector[idx]),
                        std::forward_as_tuple(get_ref(std::get<std::vector<Components &>>(query->data))...));
                return cache;
            }

            Iterator &operator++()
            {
                ++idx;
                return *this;
            }
            Iterator operator++(int)
            {
                Iterator tmp = *this;
                ++(*this);
                return tmp;
            }
            bool operator==(const Iterator &other) const
            {
                return idx == other.idx;
            }
            bool operator!=(const Iterator &other) const
            {
                return !(*this == other);
            }
        };

        Iterator begin()
        {
            return Iterator(this, 0);
        }
        Iterator end()
        {
            return Iterator(this, this->size());
        }

        void add(pid id, const Components &... components)
        {
            std::get<0>(data).push_back(id);
            std::get<std::vector<Components &>>(data).push_back(components);
        }

        Query() : data(std::make_tuple(std::vector<pid>(), std::vector<Components &>()...))
        {}

    };


}
