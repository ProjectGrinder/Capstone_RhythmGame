#include "renderer.h"

void render(RenderingEnvironment *env, const RenderObject *obj)
{
    if (env->layout != obj->input_layout)
    {
        env->layout = obj->input_layout;
        env->context->IASetInputLayout(env->layout);
    }

    if (env->topology != obj->topology)
    {
        env->topology = obj->topology;
        env->context->IASetPrimitiveTopology(env->topology);
    }

    if (env->vertex_buffer != obj->vertex_buffer || env->stride != obj->stride || env->offset != obj->offset)
    {
        env->vertex_buffer = obj->vertex_buffer;
        env->stride = obj->stride;
        env->offset = obj->offset;

        env->context->IASetVertexBuffers(0, 1, &env->vertex_buffer, &env->stride, &env->offset);
    }

    if (env->index_buffer != obj->index_buffer || env->index_format != obj->index_format)
    {
        env->index_buffer = obj->index_buffer;
        env->index_format = obj->index_format;

        env->context->IASetIndexBuffer(
                env->index_buffer, env->index_buffer ? env->index_format : DXGI_FORMAT_UNKNOWN, 0);
    }

    if (env->vertex_shader != obj->vertex_shader)
    {
        env->vertex_shader = obj->vertex_shader;
        env->context->VSSetShader(env->vertex_shader, nullptr, 0);
    }

    if (env->pixel_shader != obj->pixel_shader)
    {
        env->pixel_shader = obj->pixel_shader;
        env->context->PSSetShader(env->pixel_shader, nullptr, 0);
    }

    if (obj->index_buffer)
        env->context->DrawIndexed(obj->count.index_count, 0, 0);
    else
        env->context->Draw(obj->count.vertex_count, 0);
}
