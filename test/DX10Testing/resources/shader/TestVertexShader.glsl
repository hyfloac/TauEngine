#version 430 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;

out vec4 fColor;
out vec2 fTexCoord;

layout(binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
    mat4 model;
};

void main(void)
{
    gl_Position = projection * view * model * vec4(position, 0.0, 1.0);
    fColor = color;
    fTexCoord = texCoord;
}
