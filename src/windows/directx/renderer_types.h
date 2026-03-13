#pragma once
#include <cstdint>
#include <d3d11.h>
#include <maths.h>
#include <wrl/client.h> // Required for ComPtr

using Microsoft::WRL::ComPtr;

struct RenderObject
{
    // 1. Safety: ComPtrs automatically handle AddRef/Release
    ComPtr<ID3D11Buffer> vertex_buffer;
    ComPtr<ID3D11Buffer> index_buffer;

    ComPtr<ID3D11VertexShader> vertex_shader;
    ComPtr<ID3D11InputLayout> input_layout;
    ComPtr<ID3D11PixelShader> pixel_shader;

    UINT offset = 0;

    union
    {
        UINT index_count;
        UINT vertex_count;
    } count;

    INT vertex_base = 0;

    UINT stride = sizeof(Math::Point);
    D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
    DXGI_FORMAT index_format = DXGI_FORMAT_UNKNOWN;

    uint64_t get_sort_key() const
    {
        return (reinterpret_cast<uint64_t>(vertex_shader.Get()) << 32) |
               (reinterpret_cast<uint64_t>(pixel_shader.Get()) << 16) | reinterpret_cast<uint64_t>(input_layout.Get());
    }
};
