#version 330 core

in vec3 fPosition;
in vec2 fTexCoord;
in mat3 fTBN;

out vec4 color;
out vec4 position;
out vec4 normal;
out vec4 specular;

#include <|TERes/shader/include/Material.glsl>

layout(std140) uniform MaterialUni
{
    Material material;
};

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;
uniform sampler2D normalSampler;

void main(void)
{
    vec4 diffuseSample = texture(diffuseSampler,  fTexCoord);
    vec4 specularSample = texture(specularSampler, fTexCoord);
    vec3 normalSample = vec3(texture(normalSampler, fTexCoord));

    vec3 N = normalize(normalSample * 2.0f - 1.0f);
    N = normalize(fTBN * N);

    color = diffuseSample;
    position = vec4(fPosition, material.specularExponent);
    normal = vec4(N, 0.0f);
    specular = specularSample;
}
