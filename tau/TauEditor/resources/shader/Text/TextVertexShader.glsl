#version 420 core

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 texCoord;

out VertexData
{
    vec2 texCoord;
} vertexOut;

layout(binding = 0) uniform Matrices
{
    mat4 projectionMatrix;
};

// uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * vec4(vertex, 0.0, 1.0);
    vertexOut.texCoord = texCoord;
}
