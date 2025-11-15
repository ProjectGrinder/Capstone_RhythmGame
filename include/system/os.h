#pragma once

/*
 *  Everything that you need from OS call from
 *  this class only.
 *  (easier to control the system flow)
 */

#include <Windows.h>
#include <cstdint>

#include "system/config.h"
#include "system/renderer.h"

namespace System
{
    class OS
    {
        uint16_t _system_precision = 15;
        HINSTANCE _handler = nullptr;

        std::unique_ptr<Renderer::VertexGeneratorQueue<Config::VertexQueueSize>> _vertex_queue =
                std::make_unique<Renderer::VertexGeneratorQueue<Config::VertexQueueSize>>();

        Renderer::WindowInfo _window;
        Renderer::Directx _renderer;

        OS();
        void _sleep() const;
        void _poll_event() const;
        uint32_t _create_window();
        void _run();

    public:
        static uint32_t run();
        static OS &instance();
        static bool is_running();
        static void stop();
        static uint32_t set_system_precision(int32_t ms);
        static uint16_t get_system_precision();
        static uint32_t clean_up();
    };

    static LRESULT CALLBACK window_process(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
} // namespace System
