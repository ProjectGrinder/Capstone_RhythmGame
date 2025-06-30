#pragma once

/*
 *  Everything that you need from OS call from
 *  this class only.
 *  (easier to control the system flow)
 */

#include <Windows.h>
#include <cstdint>
#include <d3d11.h>
#include <dxgi1_4.h>
#include <wrl/client.h>

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
            int width;
            int height;
            HWND handler;
            DisplayType display_type;
            bool is_running;
            bool is_minimized;
        };

        struct Monitor
        {
            int width;
            int height;
        };

        struct Renderer
        {
            Microsoft::WRL::ComPtr<ID3D11Device> device;
            Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
            Microsoft::WRL::ComPtr<IDXGISwapChain3> swap_chain;
            Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view;
        };

        Window _window;
        Monitor _monitor;
        Renderer _renderer;
        uint16_t _system_precision = 15;
        HINSTANCE _handler = nullptr;

        OS();
        void _sleep() const;
        void _poll_event() const;
        uint32_t _create_window();
        uint32_t _initialize_directx();
        void _run();
    public:
        static uint32_t run();
        static OS &instance();
        static void minimized(bool value);
        static bool is_running();
        static void stop();
        static uint32_t set_system_precision(int32_t ms);
        static uint16_t get_system_precision();
        static uint32_t clean_up();
    };

    static LRESULT CALLBACK window_process(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
}
