struct VSInput
{
    float2 position : POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
    uint vertexID : SV_VertexID;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

cbuffer Matrices : register(b0)
{
    float4x4 projection;
    float4x4 view;
    float4x4 model;
};

PSInput vsMain(VSInput input)
{
    PSInput output;

    float4 pos = float4(input.position, 0.0f, 1.0f);
    pos = mul(pos, model);
    pos = mul(pos, view);
    pos = mul(pos, projection);

    output.position = pos;
    output.color = input.color;
    output.tex = input.tex;

    return output;
}
