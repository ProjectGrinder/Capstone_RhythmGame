#pragma once

#include <array>
#include <atomic>
#include <optional>
#include <queue>

namespace System::Renderer
{
    template<std::size_t BufferSize>
    class VertexGeneratorQueue
    {
    private:
        std::array<std::priority_queue<RenderItem>, BufferSize> _buffer;
        std::atomic<std::size_t> _current_read = {0};
        std::atomic<std::size_t> _current_write = {0};

    public:
        template<typename ResourceManager>
        void write(ResourceManager &manager)
        {
            std::priority_queue<RenderItem, std::vector<RenderItem>, std::greater<RenderItem>> items;

            if (manager.template has_resource_type<Render2D>())
            {
                for (auto &item : manager.template query<Render2D>())
                {
                    items.push(item.second);
                }
            }

            if (manager.template has_resource_type<Render3D>())
            {
                for (auto &item : manager.template query<Render3D>())
                {
                    items.push(item.second);
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

        std::optional<const std::priority_queue<RenderItem>&> read() const
        {
            const std::size_t read_idx = _current_read.load(std::memory_order_relaxed);
            const std::size_t write_idx = _current_write.load(std::memory_order_acquire);

            if (read_idx == write_idx)
            {
                return{};
            }

            return(_buffer[read_idx]);
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
