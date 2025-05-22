#pragma once

/*
 *  Everything that you need from OS call from
 *  this class only.
 *  (easier to control the system flow)
 */

#include <Windows.h>
#include <optional>
#include <exception>
#include <memory>
#include <cstdint>

namespace System
{
    enum DisplayType
    {
        FULLSCREEN,
        BORDERLESS,
        WINDOW,
    };

    class OS 
    {
    private:
        struct Window
        {
            uint32_t width;
            uint32_t height;
            HWND handler;
            DisplayType display_type;
            bool is_running;
        };

        struct Monitor
        {
            uint32_t width;
            uint32_t height;
        };

        Window _window;
        Monitor _monitor;
        uint16_t _system_precision = 15;
        HINSTANCE _handler = nullptr;

        OS();
        uint32_t _sleep();
        uint32_t _poll_event() const;
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
}
