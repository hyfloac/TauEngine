#version 430 core

in vec4 fColor;
in vec2 fTexCoord;

layout(location = 0) out vec4 color;

layout(binding = 1) uniform ColorUniform
{
    vec4 matColor;
};

layout(location = 0) uniform sampler2D shaderTex;

void main(void)
{
    vec4 texel = texture(shaderTex, fTexCoord);

    color = texel;
}
