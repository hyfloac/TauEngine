struct VSInput
{
    float3 position : POSITION;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

cbuffer Matrices : register(b0)
{
    float4x4 projection;
    float4x4 view;
};

PSInput vsMain(VSInput input)
{
    PSInput output;

    float4 pos = float4(input.position, 1.0f);
    //pos = mul(pos, view);
    //pos = mul(pos, projection);

    //output.position = pos.xyww;
    output.position = pos.xyww;
    output.texCoord = input.position;

    return output;
}
