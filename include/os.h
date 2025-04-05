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

namespace System
{
    class OS 
    {
    private:
        bool _is_running = false;
        HINSTANCE _system_instance_handler = nullptr;

        static OS _instance;

        OS(){};

    public:
        static int run();
        static OS &instance();
        static bool is_running();
        static void stop();
    };
}
