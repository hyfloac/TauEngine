struct PSInput
{
    float4 position : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

TextureCube skybox;
SamplerState skyboxSampler;

float4 psMain(PSInput input) : SV_TARGET
{
    return skybox.Sample(skyboxSampler, input.texCoord);
}
