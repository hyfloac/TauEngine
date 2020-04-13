#version 420 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

out vec2 fTexCoord;

void main(void)
{
    // 0.99999994 is the smallest representable float less than 1.0f
    gl_Position = vec4(position, 0.999999, 1.0);
    fTexCoord = texCoord;
}
