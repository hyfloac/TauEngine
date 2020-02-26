struct VSInput
{
    float2 position : POSITION;
    float2 texCoord : TEXCOORD;	
};

struct PSInput
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD;
};

cbuffer Matrices : register(b0)
{
    float4x4 projection;
};

PSInput vsMain(VSInput input)
{
    PSInput output;

    float4 pos = float4(input.position, 0.0f, 1.0f);
    pos = mul(pos, projection);

    output.position = pos;
    output.texCoord = input.texCoord;

    return output;
}
