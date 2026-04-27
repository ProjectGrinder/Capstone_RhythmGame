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
    float4 texColor = tex.Sample(samp, input.uv);
    return texColor * input.color;
}