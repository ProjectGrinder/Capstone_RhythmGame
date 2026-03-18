#pragma once
#include <d3d11.h>
#include <maths.h>
#include <wrl/client.h>

#include "system/intent_storage.h"

using Microsoft::WRL::ComPtr;

struct VertexShaderCache
{
    ComPtr<ID3D11VertexShader> shader;
    ComPtr<ID3D11InputLayout> layout;
};

struct PixelShaderCache
{
    ComPtr<ID3D11PixelShader> shader;
};

struct RenderObject
{
    ComPtr<ID3D11Buffer> vertex_buffer;
    ComPtr<ID3D11Buffer> index_buffer;

    ComPtr<ID3D11VertexShader> vertex_shader;
    ComPtr<ID3D11InputLayout> input_layout;
    ComPtr<ID3D11PixelShader> pixel_shader;

    union RenderID
    {
        struct
        {
            USHORT sp_id;
            USHORT ps_id;
            USHORT vs_id;
            UCHAR padding;
            UCHAR layer;
        } as_data;
        ULONGLONG sort_key;
    } render_id = {};

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
};

struct SpriteRenderObject
{
    ComPtr<ID3D11Buffer> vertex_buffer;
    ComPtr<ID3D11Buffer> index_buffer;

    ComPtr<ID3D11Texture2D> texture;
    ComPtr<ID3D11ShaderResourceView> texture_view;
    ComPtr<ID3D11SamplerState> sampler_state;

    ComPtr<ID3D11VertexShader> vertex_shader;
    ComPtr<ID3D11InputLayout> input_layout;
    ComPtr<ID3D11PixelShader> pixel_shader;

    union RenderID
    {
        struct
        {
            USHORT sp_id;
            USHORT ps_id;
            USHORT vs_id;
            UCHAR padding;
            UCHAR layer;
        } as_data;
        ULONGLONG sort_key;
    } render_id = {};

    UINT offset = 0;

    union
    {
        UINT index_count;
        UINT vertex_count;
    } count{};

    INT vertex_base = 0;
    UINT stride = sizeof(Math::PointUv);
    D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    DXGI_FORMAT index_format = DXGI_FORMAT_R32_UINT;
};