struct VS_INPUT
{
    float3 pos : POSITION;
    float2 uv  : TEXCOORD0;
    float4 color : COLOR0;
};

struct PS_INPUT
{
    float4 pos: SV_POSITION;
    float2 uv: TEXCOORD;
    float4 color : COLOR0;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.pos = float4(input.pos, 1.0f);
    output.uv = input.uv;
    output.color = input.color;
    return output;
}