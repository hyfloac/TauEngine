#version 330 core

in vec2 f_uv;
in vec4 f_color;

layout(location = 0) out vec4 fragColor;

uniform sampler2D textureSampler;

void main(void)
{
    fragColor = f_color * texture(textureSampler, f_uv.st);
}
