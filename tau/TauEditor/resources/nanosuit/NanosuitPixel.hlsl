struct PSInput
{
    float4 svPosition : SV_POSITION;
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
    float3x3 tbn : TBN;
};

struct PointLight
{
    float4 position;

    float4 ambient;
    float4 diffuse;
    float4 specular;

    float constant;
    float linearVal;
    float quadratic;
};

struct SpotLight
{
    float4 position;
    float4 direction;

    float4 ambient;
    float4 diffuse;
    float4 specular;

    float cutOff;
    float outerCutOff;

    float constant;
    float linearVal;
    float quadratic;
};

cbuffer Material : register(b1)
{
    float specularExponent;
}

cbuffer PointLightUni : register(b2)
{
    PointLight pointLight;
}

cbuffer SpotLightUni : register(b3)
{
    SpotLight spotLight;
}

cbuffer ViewPos : register(b4)
{
    float4 viewPos4;
}

Texture2D diffuseTexture : register(t0);
Texture2D specularTexture : register(t1);
Texture2D normalTexture : register(t2);
SamplerState textureSampler : register(s0);

float3 computePointLight(PointLight light, float3 cameraPos, float3 pos, float3 normal, float3 diffuseTexel, float3 specularTexel, float specExp);
float3 computeSpotLight(SpotLight light, float3 cameraPos, float3 pos, float3 normal, float3 diffuseTexel, float3 specularTexel, float specExp);

float4 psMain(PSInput input) : SV_TARGET
{
    float3 viewPos = viewPos4.xyz;

    float3 diffuseTexel = diffuseTexture.Sample(textureSampler, input.texCoord).rgb;
    float3 specularTexel = specularTexture.Sample(textureSampler, input.texCoord).rgb;
    float3 normalTexel = normalTexture.Sample(textureSampler, input.texCoord).rgb;

    float3 normal = normalize(normalTexel * 2.0f - 1.0f);
    float3 normalTransformed = normalize(mul(normal, input.tbn));

    float3 pointVal = computePointLight(pointLight, viewPos, input.position, normalTransformed, diffuseTexel, specularTexel, specularExponent);
    float3 spotVal  =  computeSpotLight(spotLight,  viewPos, input.position, normalTransformed, diffuseTexel, specularTexel, specularExponent);

    // float3 result = pointVal + spotVal;
    // result /= 2.0f;

    float3 result = pointVal + spotVal;

    // result = (normalTransformed + 1.0f) / 2.0f;
    // result = normalTransformed;

    return float4(result, 1.0f);
}

float3 computePointLight(PointLight light, float3 cameraPos, float3 pos, float3 normal, float3 diffuseTexel, float3 specularTexel, float specExp)
{
    float3 lightPosDelta = light.position.xyz - pos;

    float dist = length(lightPosDelta);
    float attenuation = 1.0f / (light.constant + 
                                light.linearVal * dist + 
                                light.quadratic * dist * dist);

    float3 lightDir = normalize(lightPosDelta);

    float3 viewDir = normalize(cameraPos - pos);
    float3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0f);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), specExp);

    float3 ambient  = light.ambient.rgb  *         diffuseTexel;
    float3 diffuse  = light.diffuse.rgb  * diff *  diffuseTexel * attenuation;
    float3 specular = light.specular.rgb * spec * specularTexel * attenuation;

    return ambient + diffuse + specular;
    // return specular;
}

float3 computeSpotLight(SpotLight light, float3 cameraPos, float3 pos, float3 normal, float3 diffuseTexel, float3 specularTexel, float specExp)
{
    float3 lightPosDelta = light.position.xyz - pos;
    float3 lightDir = normalize(lightPosDelta);

    float theta = dot(lightDir, normalize(-light.direction.xyz));

    if(theta > light.outerCutOff)
    {
        float epsilon = light.cutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

        float dist = length(lightPosDelta);
        float attenuation = 1.0f / (light.constant + 
                                    light.linearVal * dist + 
                                    light.quadratic * dist * dist);

        float3 viewDir = normalize(cameraPos - pos);
        float3 reflectDir = reflect(-lightDir, normal);

        float diff = max(dot(normal, lightDir), 0.0f);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), specExp);

        float3 ambient  = light.ambient.rgb  *        diffuseTexel;
        float3 diffuse  = light.diffuse.rgb  * diff * diffuseTexel  * attenuation * intensity;
        float3 specular = light.specular.rgb * spec * specularTexel * attenuation * intensity;

        return ambient + diffuse + specular;
    }
    else
    {
        return light.ambient.rgb * diffuseTexel;
    }
}
