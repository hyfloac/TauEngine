#version 420 core

in vec4 fColor;

layout(location = 0) out vec4 color;

layout(binding = 1) uniform ColorUniform
{
    vec4 matColor;
};

void main(void)
{
    color = fColor * matColor;
}
