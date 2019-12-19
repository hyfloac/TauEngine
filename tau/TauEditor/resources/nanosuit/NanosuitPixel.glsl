#version 330 core

in vec3 fPosition;
in vec3 fNormal;
in vec3 fTangent;
in vec3 fBiTangent;
in vec2 fTexCoord;

layout(location = 0) out vec4 fragColor;

#include <|TERes/shader/include/Material.glsl>
#include <|TERes/shader/include/PointLight.glsl>
#include <|TERes/shader/include/SpotLight.glsl>

uniform Material material;
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform vec3 viewPos;

#include <|TERes/shader/include/Common.glsl>
#include <|TERes/shader/include/Branchless.glsl>
// #include <|TERes/shader/include/PointLight.glsl>

void main(void)
{
    vec3 diffuseSample = vec3(texture(material.diffuseSampler,  fTexCoord));
    vec3 specularSample = vec3(texture(material.specularSampler, fTexCoord));
    vec3 point = computePointLight(pointLight, viewPos, fPosition, fNormal, diffuseSample, specularSample, material.specularExponent);
    // vec3 point = vec3(0.0);
    vec3 spot = computeSpotLight(spotLight, viewPos, fPosition, fNormal, diffuseSample, specularSample, material.specularExponent);

    vec3 result = point + spot;
    result /= 2.0;

    fragColor = vec4(result, 1.0);

    // if(gl_FragCoord.x < 400)
    // {
    //     fragColor = vec4(normalize(fNormal), 1.0);
    // }
    // else
    // {
    //     fragColor = vec4(normalize(fBiTangent), 1.0);
    // }
}
