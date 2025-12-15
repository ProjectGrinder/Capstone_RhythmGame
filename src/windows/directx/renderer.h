#pragma once

#include "renderer_types.h"

#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl/client.h>

typedef struct
{
    ID3D11DeviceContext *context;

    ID3D11InputLayout *layout;
    D3D11_PRIMITIVE_TOPOLOGY topology;

    ID3D11Buffer *vertex_buffer;
    UINT stride;
    UINT offset;

    ID3D11Buffer *index_buffer;
    DXGI_FORMAT index_format;

    ID3D11VertexShader *vertex_shader;
    ID3D11PixelShader *pixel_shader;
} RenderingEnvironment;

void render(RenderingEnvironment *env, const RenderObject *obj);
