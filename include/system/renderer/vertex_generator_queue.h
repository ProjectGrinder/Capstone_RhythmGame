#pragma once

#include "system/renderer.h"

#include <array>
#include <atomic>
#include <optional>
#include <queue>

namespace System::Renderer
{
    using ItemsQueue = std::priority_queue<RenderItem, std::vector<RenderItem>, std::greater<RenderItem>>;

    template<std::size_t BufferSize>
    class VertexGeneratorQueue
    {
        std::array<ItemsQueue, BufferSize> _buffer;
        std::atomic<std::size_t> _current_read = {0};
        std::atomic<std::size_t> _current_write = {0};

    public:
        template<typename ResourceManager>
        void write(ResourceManager *manager)
        {
            ItemsQueue items;

            if constexpr (ResourceManager::template has_resource_type<Render2D>())
            {
                for (auto entry: manager->template query<Render2D>())
                {
                    RenderItem item(static_cast<Render2D &>(entry.second));
                    items.push(item);
                }
            }

            if constexpr (ResourceManager::template has_resource_type<Render3D>())
            {
                for (auto entry: manager->template query<Render3D>())
                {
                    RenderItem item(static_cast<Render3D &>(entry.second));
                    items.push(item);
                }
            }

            std::size_t write_idx = _current_write.load(std::memory_order_relaxed);
            std::size_t read_idx = _current_read.load(std::memory_order_acquire);

            if (read_idx == write_idx)
            {
                write_idx = (write_idx + 1) % BufferSize;
            }

            this->_buffer[write_idx] = std::move(items);
            this->_current_write.store((write_idx + 1) % BufferSize, std::memory_order_release);
        }

        std::optional<ItemsQueue> read() const
        {
            const std::size_t read_idx = _current_read.load(std::memory_order_relaxed);
            const std::size_t write_idx = _current_write.load(std::memory_order_acquire);

            if (read_idx == write_idx)
            {
                return {};
            }

            return (_buffer[read_idx]);
        }

        void advance_read()
        {
            const std::size_t read_idx = _current_read.load(std::memory_order_relaxed);
            const std::size_t write_idx = _current_write.load(std::memory_order_acquire);

            if (read_idx == write_idx)
            {
                return;
            }

            _current_read.store((read_idx + 1) % BufferSize, std::memory_order_release);
        }
    };
} // namespace System::Renderer
