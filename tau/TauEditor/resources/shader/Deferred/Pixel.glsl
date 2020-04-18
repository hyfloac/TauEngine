#version 430 core

in vec3 fPosition;
in vec2 fTexCoord;
in mat3 fTBN;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 position;
layout(location = 2) out vec4 normal;
layout(location = 3) out vec4 specular;

#include <|TERes/shader/include/Material.glsl>

layout(std140, binding = 1) uniform MaterialUni
{
    Material material;
};

layout(location = 0) uniform sampler2D diffuseSampler;
layout(location = 1) uniform sampler2D specularSampler;
layout(location = 2) uniform sampler2D normalSampler;

void main(void)
{
    vec4 diffuseSample = texture(diffuseSampler,  fTexCoord);
    vec4 specularSample = texture(specularSampler, fTexCoord);
    vec3 normalSample = vec3(texture(normalSampler, fTexCoord));

    vec3 N = normalize(normalSample * 2.0 - 1.0);
    N = normalize(fTBN * N);

    color = diffuseSample;
    position = vec4(fPosition, material.specularExponent);
    normal = vec4(N, 0.0f);
    specular = specularSample;
}
