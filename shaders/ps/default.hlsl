Texture2D tex : register(t0);
SamplerState samp : register(s0);

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD;
    float4 color : Color;
};

float4 main(PS_INPUT input) : SV_Target
{
    return tex.Sample(samp, input.uv);
}