#version 420 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;

out vec4 fColor;

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
}
