struct PSInput
{
    float4 svPosition : SV_Position;
    float2 texCoord : TexCoord;
};

struct PSOutput
{
    float4 color : SV_Target0;
    float depth : SV_Depth;
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

cbuffer PointLightUni : register(b0)
{
    PointLight pointLight;
}

cbuffer SpotLightUni : register(b1)
{
    SpotLight spotLight;
}

cbuffer ViewPos : register(b2)
{
    float4 viewPos4;
}

Texture2D<float4> colorTexture : register(t0);
Texture2D<float4> positionTexture : register(t1);
Texture2D<float4> normalTexture : register(t2);
Texture2D<float4> specularTexture : register(t3);
Texture2D<float> depthTexture : register(t4);
SamplerState textureSampler : register(s0);

float3 computePointLight(PointLight light, float3 cameraPos, float3 pos, float3 normal, float3 diffuseTexel, float3 specularTexel, float specExp);
float3 computeSpotLight(SpotLight light, float3 cameraPos, float3 pos, float3 normal, float3 diffuseTexel, float3 specularTexel, float specExp);

PSOutput psMain(PSInput input)
{
    float3 viewPos = viewPos4.xyz;

    float3 colorTexel = colorTexture.Sample(textureSampler, input.texCoord).rgb;
    float4 positionTexel = positionTexture.Sample(textureSampler, input.texCoord);
    float3 normalTexel = normalTexture.Sample(textureSampler, input.texCoord).rgb;
    float4 specularTexel = specularTexture.Sample(textureSampler, input.texCoord);

    float specExp = positionTexel.w;

    float3 pointVal = computePointLight(pointLight, viewPos, positionTexel.xyz, normalTexel, colorTexel, specularTexel.rgb, specExp);
    float3 spotVal  =  computeSpotLight(spotLight,  viewPos, positionTexel.xyz, normalTexel, colorTexel, specularTexel.rgb, specExp);

    float3 result = pointVal + spotVal;

    PSOutput output;
    output.color = float4(result, 1.0f);
    output.depth = depthTexture.Sample(textureSampler, input.texCoord);

    return output;
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
