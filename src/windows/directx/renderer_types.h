#include <d3d11.h>

/* Took ownership of the rendering info */
typedef struct
{
    ID3D11Buffer *vertex_buffer;
    ID3D11Buffer *index_buffer;

    ID3D11VertexShader *vertex_shader;
    ID3D11InputLayout *input_layout;
    ID3D11PixelShader *pixel_shader;

    UINT stride;
    UINT offset;
    union
    {
        UINT index_count;
        UINT vertex_count;
    } count;

    D3D11_PRIMITIVE_TOPOLOGY topology;
    DXGI_FORMAT index_format;
} RenderObject;
