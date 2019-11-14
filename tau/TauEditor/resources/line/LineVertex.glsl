#version 330 core

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec4 color;

out VertexData
{
    vec4 mColor;
} vertexOut;

uniform mat4 viewMatrix;

void main(void)
{
    vertexOut.mColor = color;
    gl_Position = viewMatrix * vec4(vertex, 0.0f, 0.0f);
}
