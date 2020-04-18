struct PSInput
{
    float4 svPosition : SV_Position;
    float3 position : Position;
    float2 texCoord : TexCoord;
    float3x3 tbn : TBN;
};

struct PSOutput
{
    float4 color : SV_Target0;
    float4 position : SV_Target1;
    float4 normal : SV_Target2;
    float4 specular : SV_Target3;
};

cbuffer Material : register(b1)
{
    float specularExponent;
}

Texture2D<float4> diffuseTexture : register(t0);
Texture2D<float4> specularTexture : register(t1);
Texture2D<float3> normalTexture : register(t2);
SamplerState textureSampler : register(s0);

PSOutput psMain(PSInput input)
{
    float4 diffuseTexel = diffuseTexture.Sample(textureSampler, input.texCoord);
    float4 specularTexel = specularTexture.Sample(textureSampler, input.texCoord);
    float3 normalTexel = normalTexture.Sample(textureSampler, input.texCoord);

    float3 N = normalize(normalTexel * 2.0f - 1.0f);
    N = normalize(mul(N, input.tbn));

    PSOutput output;
    output.color = diffuseTexel;
    output.position = float4(input.position, specularExponent);
    output.normal = float4(N, 0.0f);
    output.specular = specularTexel;

    return output;
}
