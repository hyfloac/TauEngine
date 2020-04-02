struct PSInput
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

cbuffer Color : register(b1)
{
    float4 textColor;
};

Texture2D<float> textBMP;
SamplerState textBMPSampler;

float4 psMain(PSInput input) : SV_TARGET
{
    float texel = textBMP.Sample(textBMPSampler, input.texCoord);
    
    return float4(textColor.xyz, texel);
}
