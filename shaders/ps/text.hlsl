Texture2D tex : register(t0);
SamplerState samp : register(s0);

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD;
    float4 color : COLOR;
};

float4 main(PS_INPUT input) : SV_Target
{
    float mask = tex.Sample(samp, input.uv).r;
    input.color.a *= mask;
    return input.color;
}