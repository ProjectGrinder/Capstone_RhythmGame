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
    class OS 
    {
    private:
        bool _is_running = false;
        uint16_t _system_precision = 15;
        HINSTANCE _system_instance_handler = nullptr;
        std::unique_ptr<Window> _window;
        static OS _instance;

        OS();
        static uint32_t _sleep();
        static uint32_t _poll_event();

    public:
        static uint32_t run();
        static OS &instance();
        static bool is_running();
        static void stop();
        static uint32_t set_system_precision(int32_t ms);
        static uint16_t get_system_precision();
    };
}
