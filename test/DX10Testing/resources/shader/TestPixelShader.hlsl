struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

cbuffer ColorUniform : register(b1)
{
    float4 color;
};

Texture2D shaderTex;
SamplerState shaderTexSampler;
// {
//     Filter = MIN_MAG_MIP_POINT;
//     AddressU = Wrap;
//     AddressV = Wrap;
// };

float4 psMain(PSInput input) : SV_TARGET
{
    float4 texel = shaderTex.Sample(shaderTexSampler, input.tex);

    return texel;
    // return float4(input.tex, 0.0f, 1.0f);
    // return input.color * color;
    // return input.color;
}
