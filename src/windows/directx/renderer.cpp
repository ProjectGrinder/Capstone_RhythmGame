#include "renderer.h"

void render(RenderingEnvironment *env, const RenderObject *obj)
{
    ID3D11InputLayout *current_layout = obj->input_layout.Get();
    ID3D11VertexShader *current_vs = obj->vertex_shader.Get();
    ID3D11PixelShader *current_ps = obj->pixel_shader.Get();
    ID3D11ShaderResourceView *texture_view = obj->texture_view.Get();
    ID3D11SamplerState *sampler_state = obj->sampler_state.Get();

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

    if (env->vertex_shader != current_vs)
    {
        env->vertex_shader = current_vs;
        env->context->VSSetShader(env->vertex_shader, nullptr, 0);
    }

    if (env->pixel_shader != current_ps)
    {
        env->pixel_shader = current_ps;
        env->context->PSSetShader(env->pixel_shader, nullptr, 0);
    }

    //    if (obj->render_id.as_data.sp_id == 0)
    //    {
    //        goto _render;
    //    }

    /* Assets Change */
    if (env->texture_view != texture_view)
    {
        env->texture_view = texture_view;
        env->context->PSSetShaderResources(0, 1, &texture_view);
    }

    if (env->sampler_state != sampler_state)
    {
        env->sampler_state = sampler_state;
        env->context->PSSetSamplers(0, 1, &sampler_state);
    }

    //_render:
    env->context->DrawIndexed(obj->count.index_count, obj->offset, obj->vertex_base);
}
