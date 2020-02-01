struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

struct PointLight
{
    float3 pos;
    float3 ambient;
    float3 diffuse;
    float3 specular;

    float constant;
    float linearVal;
    float quadratic;
};

cbuffer LightsBuffer
{
    PointLight pointLights[8];
    float3 viewPos;
    float specExp;
};

Texture2D diffuseTex;
Texture2D specularTex;
SamplerState textureSampler 
{
    Filter = MIN_MAG_MIP_LINEAR;
};

float3 computePointLight(PointLight light, float3 cameraPos, float3 pos, float3 normal, float3 diffuseSample, float3 specularSample, float specExp);

float4 main(PSInput input) : SV_TARGET
{
    float3 normal = normalize(input.normal);

    float3 diffuseSample = diffuseTex.Sample(textureSampler, input.tex).rgb;
    float3 specularSample = specularTex.Sample(textureSampler, input.tex).rgb;

    float3 color;

    for(int i = 0; i < 8; ++i)
    {
        color += computePointLight(pointLights[i], viewPos, input.position, normal, diffuseSample, specularSample, specExp);
    }

    return float4(color, 1.0);
}

float3 computePointLight(PointLight light, float3 cameraPos, float3 pos, float3 normal, float3 diffuseSample, float3 specularSample, float specExp)
{
    float3 lightPosDelta = light.pos - pos;

    float dist = length(lightPosDelta);
    float attenuation = 1.0 / (light.constant + 
                               light.linearVal * dist +
                               light.quadratic * dist * dist);

    float3 lightDir = normalize(lightPosDelta);

    float3 viewDir = normalize(cameraPos - pos);
    float3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specExp);

    float3 ambient  = light.ambient  *        diffuseSample;
    float3 diffuse  = light.diffuse  * diff * diffuseSample  * attenuation;
    float3 specular = light.specular * spec * specularSample * attenuation;

    return ambient + diffuse + specular;
};
