struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

cbuffer ColorUniform : register(b1)
{
    float4 color;
};

float4 psMain(PSInput input) : SV_TARGET
{
    return input.color * color;
    // return input.color;
}
