struct PSInput
{
    float4 svPosition : SV_Position;
    float3 position : Position;
    float2 texCoord : TexCoord;
    float3x3 TBN : TBN;
};

static const float PI = 3.14159265258f;
static const float Epsilon = 0.00001f;
static const float3 FresnelDielectric = float3(0.04f);

static const uint NumLights = 6;

cbuffer Uniforms : register(b0)
{
    struct {
        float3 direction;
        float3 color;
    } lights[NumLights];
    float3 eyePosition;
}

Texture2D<float3> diffuseTexture : register(t0);
Texture2D<float3> normalTexture : register(t1);
Texture2D<float> metalnessTexture : register(t2);
Texture2D<float> roughnessTexture : register(t3);
Texture2D<float2> specularBRDFTexture : register(t4);

TextureCube<float3> specularMap : register(t5);
TextureCube<float3> irradianceMap : register(t6);

SamplerState textureSampler : register(s0);
SamplerState brdfSampler : register(s1);

float4 psMain(PSInput input) : SV_TARGET
{
    float3 diffuseTexel = diffuseTexture.Sample(textureSampler, input.texCoord);
    float3 normalTexel = normalTexture.Sample(textureSampler, input.texCoord);
    float metalnessTexel = metalnessTexture.Sample(textureSampler, input.texCoord);
    float roughnessTexel = roughnessTexture.Sample(textureSampler, input.texCoord);

    float3 N = normalize(normalTexel * 2.0f - 1.0f);
    N = normalize(mul(input.tbn, normal));

    float3 Lo = normalize(eyePosition - input.position);

    float cosLo = max(0.0f, dot(normalTransformed, Lo));

    float3 Lr = 2.0f * cosEyeDir * normalTransormed - eyeDir;

    float3 F0 = lerp(FresnelDielectric, diffuseTexel, metalnessTexel);

    float3 directLighting = 0.0f;

    for(uint i = 0; i < NumLights; ++i)
    {
        float3 Li = -lights[i].direction;
        float3 Lcolor = lights[i].color;

        float3 Lh = normalize(Li + Lo);

        float cosLi = max(0.0f, dot(N, Li));
        float cosLh = max(0.0f, dot(N, Lh));

        
    }
}
