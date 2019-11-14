#version 330 core

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 texCoord;

out VertexData
{
    vec2 texCoord;
} vertexOut;

uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * vec4(vertex.xy, 0.0, 1.0);
    vertexOut.texCoord = texCoord;
}
