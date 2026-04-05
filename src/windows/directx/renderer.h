#pragma once

#include <d3d11.h>
#include "renderer_types.h"

struct RenderingEnvironment
{
    ID3D11DeviceContext *context = nullptr;

    ID3D11InputLayout *layout = nullptr;
    ID3D11VertexShader *vertex_shader = nullptr;
    ID3D11PixelShader *pixel_shader = nullptr;
    ID3D11Buffer *vertex_buffer = nullptr;
    ID3D11Buffer *index_buffer = nullptr;

    D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
    DXGI_FORMAT index_format = DXGI_FORMAT_UNKNOWN;
    UINT stride = 0;
    UINT offset = 0;

    void reset()
    {
        layout = nullptr;
        vertex_shader = nullptr;
        pixel_shader = nullptr;
        vertex_buffer = nullptr;
        index_buffer = nullptr;
        topology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
        index_format = DXGI_FORMAT_UNKNOWN;
    }
};

void render(RenderingEnvironment *env, const RenderObject *obj);
void render_sprite(RenderingEnvironment *env, const SpriteRenderObject *obj);
