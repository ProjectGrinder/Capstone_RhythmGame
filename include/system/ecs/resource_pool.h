#pragma once
#include <algorithm>
#include <bitset>
#include <execution>
#include "ecs_types.h"

namespace System::ECS
{
    template<std::size_t MaxResource, typename Resource>
    class ResourcePool
    {
        std::vector<Resource> _data;
        std::vector<pid> _index_to_id;
        std::vector<size_t> _id_to_index;
        std::bitset<MaxResource> _has_component;

    private:
        struct Iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::pair<pid, Resource &>;
            using difference_type = std::ptrdiff_t;
            using pointer = value_type *;
            using reference = value_type &;

            Iterator() : _pool(nullptr), _idx(SIZE_MAX)
            {}
            explicit Iterator(ResourcePool *pool, size_t idx = 0) : _pool(pool), _idx(idx)
            {}

            value_type operator*()
            {
                return {_pool->_index_to_id[_idx], _pool->_data[_idx]};
            }

            Iterator &operator++()
            {
                ++_idx;
                return (*this);
            }
            bool operator==(const Iterator &other) const
            {
                return (_idx == other._idx);
            }
            bool operator!=(const Iterator &other) const
            {
                return !(*this == other);
            }

        private:
            ResourcePool *_pool;
            size_t _idx = 0;
        };

    public:
        ResourcePool()
        {
            _id_to_index.resize(MaxResource, SIZE_MAX);
        }

        Resource &get(pid id)
        {
            return (_data[_id_to_index.at(id)]);
        }

        [[nodiscard]] bool has(pid id) const noexcept
        {
            return (id < _id_to_index.size() && _has_component.test(id));
        }

        Iterator begin()
        {
            return (Iterator(this));
        }

        Iterator end()
        {
            return (Iterator(this, _data.size()));
        }

        void add(pid id, const Resource &value)
        {
            if (_has_component.test(id))
            {
                throw std::runtime_error("Component already exists");
            }

            size_t idx = _data.size();
            _data.push_back(value);
            _id_to_index[id] = idx;
            _index_to_id.push_back(id);
            _has_component.set(id);
        }

        void remove(pid id)
        {
            size_t index = _id_to_index.at(id);
            size_t last = _data.size() - 1;

            if (index != last)
            {
                _data[index] = std::move(_data[last]);
                pid last_id = _index_to_id[last];
                _index_to_id[index] = last_id;
                _id_to_index[last_id] = index;
            }

            _data.pop_back();
            _index_to_id.pop_back();
            _id_to_index[id] = SIZE_MAX;
            _has_component.reset(id);
        }

        void clear()
        {
            _data.clear();
            _index_to_id.clear();
            _id_to_index.assign(MaxResource, SIZE_MAX);
            _has_component.reset();
        }

        // type hints bless thee
        using resource_type = Resource;
    };
} // namespace System::ECS
