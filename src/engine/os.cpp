#include <assert.h>
#include "scene/scene_manager.h"
#include "system.h"
#include "utils.h"

#ifndef PROJECT_NAME
    #define PROJECT_NAME NO_NAME_PROJECT
#endif
#define STR(name) #name
#define TO_STR(name) STR(name)

std::vector<char> read_shader(const std::string_view path)
{
    std::vector<char> data;
    uint32_t error = ERROR_SUCCESS;
    LARGE_INTEGER size = {0};
    DWORD bytesRead = 0;

    const HANDLE hFile = CreateFileA(
            path.data(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        error = GetLastError();
        LOG_DEBUG("Error code: {}, CreateFileA failed", error);
        goto Exit;
    }

    if (GetFileSizeEx(hFile, &size) == 0)
    {
        error = GetLastError();
        LOG_DEBUG("Error code: {}, GetFileSizeEx failed", error);
        goto Error;
    }

    if (size.QuadPart == 0)
    {
        error = ERROR_EMPTY;
        LOG_DEBUG("File is empty");
        goto Error;
    }

    if (size.QuadPart > SIZE_MAX)
    {
        error = ERROR_FILE_TOO_LARGE;
        LOG_DEBUG("Error code: {}, File is too large", error);
        goto Error;
    }

    data.resize(static_cast<size_t>(size.QuadPart));

    if (ReadFile(hFile, data.data(), static_cast<DWORD>(size.QuadPart), &bytesRead, nullptr) == 0 ||
        bytesRead != static_cast<DWORD>(size.QuadPart))
    {
        error = GetLastError();
        LOG_DEBUG("Error code: {}, ReadFile failed", error);
        goto Error;
    }
Error:
    if (CloseHandle(hFile) == 0)
    {
        error = GetLastError();
        LOG_DEBUG("Error code: {}, CloseHandle failed", error);
    }
Exit:
    return (data);
}

using System::OS;

OS::OS() : _window{1280, 720, nullptr, DisplayType::WINDOW, false}, _monitor{0, 0}
{
    this->_handler = GetModuleHandleA(nullptr);
    if (this->_handler == nullptr)
    {
        LOG_DEBUG("Error code: {}, GetModuleHandle failed.", GetLastError());
        throw std::runtime_error("Unable to get current instance handler.\nMore detail please check on logs");
    }

    if (timeBeginPeriod(1) != TIMERR_NOERROR)
    {
        LOG_DEBUG("timeBeginPeriod failed.");
        throw std::runtime_error("Unable to set time resolution.\nMore detail please check on logs");
    }
}

OS &OS::instance()
{
    static OS _instance;
    return (_instance);
}

uint32_t OS::run()
{
    uint32_t error = ERROR_SUCCESS;

    error = instance()._create_window();
    if (error != ERROR_SUCCESS)
        goto Exit;

    error = instance()._initialize_directx();
    if (error != ERROR_SUCCESS)
        goto Exit;

    instance()._run();

Exit:
    return (error);
}

uint32_t OS::_initialize_directx()
{
    // TODO: Make this function pure
    uint32_t error = ERROR_SUCCESS;

    using Microsoft::WRL::ComPtr;

    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    ComPtr<IDXGIDevice> dxgi_device;
    ComPtr<IDXGIAdapter> adapter;
    ComPtr<IDXGIFactory2> factory;
    ComPtr<IDXGISwapChain1> swap_chain1;
    ComPtr<IDXGISwapChain3> final_swap_chain;
    ComPtr<ID3D11Texture2D> backbuffer;
    ComPtr<ID3D11RenderTargetView> rtv;

    D3D_FEATURE_LEVEL level = {};
    D3D_FEATURE_LEVEL feature_levels[] = {D3D_FEATURE_LEVEL_11_0};

    DXGI_SWAP_CHAIN_DESC1 sc_desc = {0};
    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fs_desc = {0};

    sc_desc.BufferCount = 3; // <- Triple buffering
    sc_desc.Width = _window.width;
    sc_desc.Height = _window.height;
    sc_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sc_desc.Stereo = false;
    sc_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sc_desc.SampleDesc.Count = 1;
    sc_desc.Scaling = DXGI_SCALING_STRETCH;
    sc_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sc_desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
    sc_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

    fs_desc.Windowed = _window.display_type == DisplayType::WINDOW;

#ifdef _DEBUG
    error = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_SINGLETHREADED,
            feature_levels,
            _countof(feature_levels),
            D3D11_SDK_VERSION,
            &device,
            &level,
            &context);
#else
    error = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            D3D11_CREATE_DEVICE_SINGLETHREADED,
            feature_levels,
            _countof(feature_levels),
            D3D11_SDK_VERSION,
            &device,
            &level,
            &context);
#endif
    if (FAILED(error))
        goto Exit;

    error = device.As(&dxgi_device);
    if (FAILED(error))
        goto Exit;

    error = dxgi_device->GetAdapter(&adapter);
    if (FAILED(error))
        goto Exit;

    error = adapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void **>(factory.GetAddressOf()));
    if (FAILED(error))
        goto Exit;

    error = factory->CreateSwapChainForHwnd(device.Get(), _window.handler, &sc_desc, &fs_desc, nullptr, &swap_chain1);
    if (FAILED(error))
        goto Exit;

    error = swap_chain1.As(&final_swap_chain);
    if (FAILED(error))
        goto Exit;

    error = final_swap_chain->GetBuffer(
            0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(backbuffer.GetAddressOf()));
    if (FAILED(error))
        goto Exit;

    error = device->CreateRenderTargetView(backbuffer.Get(), nullptr, &rtv);
    if (FAILED(error))
        goto Exit;

    _renderer.device = device;
    _renderer.context = context;
    _renderer.swap_chain = final_swap_chain;
    _renderer.render_target_view = rtv;

Exit:
    return (error);
}

bool OS::is_running()
{
    return (instance()._window.is_running);
}

void OS::stop()
{
    instance()._window.is_running = false;
}

uint32_t OS::set_system_precision(int32_t ms)
{
    if (ms <= 0)
    {
        Utils::print_debug(
                "engine/os.cpp",
                __FUNCTION__,
                "Error code: {}, Cannot set system time precision to value less or equal to 0.",
                ERROR_INVALID_PARAMETER);
        return (ERROR_INVALID_PARAMETER);
    }
    else
    {
        instance()._system_precision = static_cast<uint16_t>(ms);
        return (ERROR_SUCCESS);
    }
}

void OS::_sleep() const
{
    /* This function will always succeed after Win XP */
    LARGE_INTEGER start, end, frequency;

    QueryPerformanceFrequency(&frequency);

    QueryPerformanceCounter(&start);

    do
    {
        QueryPerformanceCounter(&end);
        Sleep(0);
    }
    while ((end.QuadPart - start.QuadPart) * 1000 / frequency.QuadPart < this->_system_precision);
}

struct Vertex
{
    float position[3];
    float color[4];
};

void OS::_run()
{
    _window.is_running = true;
    constexpr float clear_color[4] = {0.39f, 0.58f, 0.93f, 1.0f};

    constexpr Vertex vertices[] = {
            {{0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
            {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {nullptr};
    initData.pSysMem = vertices;

    using namespace Microsoft::WRL;
    ComPtr<ID3D11Buffer> vertexBuffer;
    assert(SUCCEEDED(_renderer.device->CreateBuffer(&bd, &initData, &vertexBuffer)));

    ComPtr<ID3D11VertexShader> vertex_shader = nullptr;
    ComPtr<ID3D11PixelShader> pixel_shader = nullptr;

    std::vector<char> vertex_shader_code = read_shader("shaders/vs/rainbow.cso");
    std::vector<char> pixel_shader_code = read_shader("shaders/ps/rainbow.cso");

    assert(SUCCEEDED(_renderer.device->CreateVertexShader(vertex_shader_code.data(), vertex_shader_code.size(), nullptr, &vertex_shader)));
    assert(SUCCEEDED(_renderer.device->CreatePixelShader(pixel_shader_code.data(), pixel_shader_code.size(), nullptr, &pixel_shader)));

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, color), D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    ComPtr<ID3D11InputLayout> inputLayout;
    _renderer.device->CreateInputLayout(
        layout, ARRAYSIZE(layout),
        vertex_shader_code.data(), vertex_shader_code.size(),
        &inputLayout
    );

    while (_window.is_running)
    {
        /*  Polling Event  */
        _poll_event();
        /*  Update Game  */
        Scene::SceneManager::update();
        /*  Render  */
        _renderer.context->ClearRenderTargetView(_renderer.render_target_view.Get(), clear_color);

        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.Width = static_cast<float>(_window.width);
        viewport.Height = static_cast<float>(_window.height);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        _renderer.context->RSSetViewports(1, &viewport);

        _renderer.context->OMSetRenderTargets(1, _renderer.render_target_view.GetAddressOf(), nullptr);

        UINT stride = sizeof(Vertex);
        UINT offset = 0;

        _renderer.context->IASetInputLayout(inputLayout.Get());
        _renderer.context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
        _renderer.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        _renderer.context->VSSetShader(vertex_shader.Get(), nullptr, 0);
        _renderer.context->PSSetShader(pixel_shader.Get(), nullptr, 0);
        _renderer.context->Draw(3, 0);

        if (HRESULT hr = _renderer.swap_chain->Present(1, 0); FAILED(hr))
        {
            LOG_DEBUG("Failed to present swap chain: HRESULT=0x{:X}", hr);
            break;
        }

        _sleep();
    }
}

void OS::_poll_event() const
{
    MSG msg;
    while (PeekMessage(&msg, this->_window.handler, 0, 0, PM_REMOVE))
    {
        switch (msg.message)
        {
        case WM_KEYDOWN:
            Input::set_key_down(static_cast<uint8_t>(msg.wParam));
            break;
        case WM_KEYUP:
            Input::set_key_up(static_cast<uint8_t>(msg.wParam));
            break;
        case WM_MOUSEMOVE:
            Input::set_mouse_position(static_cast<uint16_t>(msg.pt.x), static_cast<uint16_t>(msg.pt.y));
            break;
        default:
            break;
        }

        DispatchMessageA(&msg);
    }
}

uint16_t OS::get_system_precision()
{
    return (instance()._system_precision);
}

uint32_t OS::_create_window()
{
    MONITORINFO monitor_info = {0};
    WNDCLASSEXA window_settings = {0};
    DEVMODEA devmode_screen_settings = {0};

    constexpr DWORD style_flags = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;

    window_settings.cbSize = sizeof(WNDCLASSEXA);
    window_settings.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_settings.lpfnWndProc = window_process;
    window_settings.cbClsExtra = 0;
    window_settings.cbWndExtra = 0;
    window_settings.hInstance = this->_handler;
    window_settings.hIcon = LoadIconA(nullptr, IDI_WINLOGO);
    window_settings.hIconSm = LoadIconA(nullptr, IDI_WINLOGO);
    window_settings.hCursor = LoadCursorA(nullptr, IDC_ARROW);
    window_settings.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    window_settings.lpszMenuName = nullptr;
    window_settings.lpszClassName = TO_STR(PROJECT_NAME);

    monitor_info.cbSize = sizeof(MONITORINFO);

    uint32_t error = ERROR_SUCCESS;

    if (RegisterClassExA(&window_settings) == 0)
    {
        error = GetLastError();
        LOG_DEBUG("Error code: {}, Failed to register window.", error);
        goto Exit;
    }

    this->_window.handler = CreateWindowExA(
            0,
            window_settings.lpszClassName,
            window_settings.lpszClassName,
            style_flags,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            nullptr,
            nullptr,
            this->_handler,
            nullptr);

    if (this->_window.handler == nullptr)
    {
        error = GetLastError();
        LOG_DEBUG("Error code: {}, Failed to create window.", error);
        goto Exit;
    }

    /* We can do this because MonitorFromWindow Guarantee the return value */
    if (GetMonitorInfoA(MonitorFromWindow(this->_window.handler, MONITOR_DEFAULTTOPRIMARY), &monitor_info) == 0)
    {
        error = ERROR_MONITOR_NO_DESCRIPTOR;

        LOG_DEBUG("Error code: {}, GetMonitorInfo failed.", error);
        goto Exit;
    }

    this->_monitor.height = monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top;
    this->_monitor.width = monitor_info.rcMonitor.right - monitor_info.rcMonitor.left;

    if (this->_window.width == 0)
    {
        this->_window.width = this->_monitor.width;
    }

    if (this->_window.height == 0)
    {
        this->_window.height = this->_monitor.height;
    }

    if (this->_window.width > this->_monitor.width || this->_window.height > this->_monitor.height)
    {
        LOG_DEBUG("Info: Either window width or window height is larger than monitor's value. Resetting");

        this->_window.width = this->_monitor.width;
        this->_window.height = this->_monitor.height;
    }

    LOG_DEBUG("Info: Monitor: {}x{}", this->_monitor.width, this->_monitor.height);
    LOG_DEBUG("Info: Window: {}x{}", this->_window.width, this->_window.height);

    switch (this->_window.display_type)
    {
    case FULLSCREEN:

        devmode_screen_settings.dmSize = sizeof(DEVMODEA);
        devmode_screen_settings.dmPelsWidth = this->_window.width;
        devmode_screen_settings.dmPelsHeight = this->_window.height;
        devmode_screen_settings.dmBitsPerPel = 32;
        devmode_screen_settings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        error = ChangeDisplaySettingsA(&devmode_screen_settings, CDS_FULLSCREEN);

        if (error != DISP_CHANGE_SUCCESSFUL)
        {
            LOG_DEBUG("Error code: {}, ChangeDisplaySettings failed.", error);

            goto Exit;
        }

        if (SetWindowPos(
                    this->_window.handler,
                    HWND_TOP,
                    monitor_info.rcMonitor.left,
                    monitor_info.rcMonitor.top,
                    this->_window.width,
                    this->_window.height,
                    SWP_NOOWNERZORDER | SWP_FRAMECHANGED) == 0)
        {
            error = GetLastError();

            LOG_DEBUG("Error code: {}, SetWindowPos failed.", error);
            goto Exit;
        }

        break;

    case BORDERLESS:

        if (SetWindowPos(
                    this->_window.handler,
                    HWND_TOP,
                    monitor_info.rcMonitor.left,
                    monitor_info.rcMonitor.top,
                    this->_monitor.width,
                    this->_monitor.height,
                    SWP_NOOWNERZORDER | SWP_FRAMECHANGED) == 0)
        {
            error = GetLastError();

            LOG_DEBUG("Error code: {}, SetWindowPos failed.", error);
            goto Exit;
        }

        break;

    case WINDOW:

        if (SetWindowLongPtrA(
                    this->_window.handler, GWL_STYLE, (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX)) == 0)
        {
            error = GetLastError();

            LOG_DEBUG("Error code: {}, SetWindowLongPtr failed.", error);
            goto Exit;
        }

        if (SetWindowPos(
                    this->_window.handler,
                    HWND_TOP,
                    (this->_monitor.width - this->_window.width) / 2,
                    (this->_monitor.height - this->_window.height) / 2,
                    this->_window.width,
                    this->_window.height,
                    SWP_NOOWNERZORDER | SWP_FRAMECHANGED) == 0)
        {
            error = GetLastError();

            LOG_DEBUG("Error code: {}, SetWindowPos failed.", error);
            goto Exit;
        }

        break;

    default:

        error = ERROR_INVALID_PARAMETER;
        LOG_DEBUG("Error code: {}, Unknown display type.", error);
        goto Exit;
    }

    ShowWindow(this->_window.handler, SW_SHOW);

Exit:
    return (error);
}

static LRESULT CALLBACK System::window_process(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
    switch (msg)
    {
    case WM_DESTROY:
    case WM_CLOSE:
        OS::stop();
        PostQuitMessage(0);
        return (0);
    default:
        return (DefWindowProc(hwnd, msg, w_param, l_param));
    }
}

uint32_t OS::clean_up()
{
    uint32_t error = ERROR_SUCCESS;
    if (DestroyWindow(instance()._window.handler) == 0)
    {
        error = GetLastError();

        LOG_DEBUG("Error code: {}, Unable to destroy window.", error);
    }

    if (instance()._window.display_type != FULLSCREEN)
    {
        goto Exit;
    }

    error = ChangeDisplaySettingsA(nullptr, 0);
    if (error != DISP_CHANGE_SUCCESSFUL)
    {
        LOG_DEBUG("Error code: {}, ChangeDisplaySetting failed.", error);
    }

    if (timeEndPeriod(1) != TIMERR_NOERROR)
    {
        LOG_DEBUG("timeEndPeriod failed.");
    }

Exit:
    return (error);
}
