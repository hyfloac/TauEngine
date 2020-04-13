#version 420 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

out vec2 fTexCoord;

void main(void)
{
    // 0.999999f is very close, but not at the far frustum
    gl_Position = vec4(position, 0.999999, 1.0);
    fTexCoord = texCoord;
}
