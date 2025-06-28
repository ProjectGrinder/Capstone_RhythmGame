struct VS_INPUT
{
    float3 pos: POSITION;
    float4 color: COLOR;
};

struct PS_INPUT
{
    float4 pos: SV_POSITION;
    float4 color: COLOR;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.pos = float4(input.pos, 1.0f);
    output.color = input.color;
    return output;
}