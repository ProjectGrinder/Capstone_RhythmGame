#include "renderer.h"

void render(RenderingEnvironment *env, const RenderObject *obj)
{
    ID3D11InputLayout *current_layout = obj->input_layout.Get();
    if (env->layout != current_layout)
    {
        env->layout = current_layout;
        env->context->IASetInputLayout(env->layout);
    }

    if (env->topology != obj->topology)
    {
        env->topology = obj->topology;
        env->context->IASetPrimitiveTopology(env->topology);
    }

    //    ID3D11Buffer *current_vb = obj->vertex_buffer.Get();
    //    if (env->vertex_buffer != current_vb || env->stride != obj->stride || env->offset != obj->offset)
    //    {
    //        env->vertex_buffer = current_vb;
    //        env->stride = obj->stride;
    //        env->offset = obj->offset;
    //
    //        env->context->IASetVertexBuffers(0, 1, &env->vertex_buffer, &env->stride, &env->offset);
    //    }
    //
    //    ID3D11Buffer *current_ib = obj->index_buffer.Get();
    //    if (env->index_buffer != current_ib || env->index_format != obj->index_format)
    //    {
    //        env->index_buffer = current_ib;
    //        env->index_format = obj->index_format;
    //
    //        env->context->IASetIndexBuffer(
    //                env->index_buffer, env->index_buffer ? env->index_format : DXGI_FORMAT_UNKNOWN, 0);
    //    }
    //
    ID3D11VertexShader *current_vs = obj->vertex_shader.Get();
    if (env->vertex_shader != current_vs)
    {
        env->vertex_shader = current_vs;
        env->context->VSSetShader(env->vertex_shader, nullptr, 0);
    }

    ID3D11PixelShader *current_ps = obj->pixel_shader.Get();
    if (env->pixel_shader != current_ps)
    {
        env->pixel_shader = current_ps;
        env->context->PSSetShader(env->pixel_shader, nullptr, 0);
    }

    //    if (current_ib)
    env->context->DrawIndexed(obj->count.index_count, obj->offset, obj->vertex_base);
    //    else
    //        env->context->Draw(obj->count.vertex_count, 0);
}
