struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

cbuffer MatrixBuffer : register(b0)
{
    float4x4 projection;
    float4x4 view;
};

cbuffer ObjectMatrixe : register(b1)
{
    float4x4 objView;
};

PSInput main(VSInput input)
{
    PSInput output;

    float4 pos = float4(input.position, 1.0f);

    pos = mul(pos, objView);
    pos = mul(pos, view);
    pos = mul(pos, projection);

    output.position = pos;
    output.normal = normalize(input.normal);
    output.tex = input.tex;

    return output;
}
