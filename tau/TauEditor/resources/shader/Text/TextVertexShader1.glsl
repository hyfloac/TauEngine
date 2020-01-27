#version 330 core

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec2 position;
layout(location = 3) in vec2 scale;


out VertexData
{
    vec2 texCoord;
} vertexOut;

uniform mat4 projectionMatrix;

void main()
{
    vec2 scaledVertex = vertex * scale;
    vec2 translatedPosition = scaledVertex + position;
    gl_Position = projectionMatrix * vec4(translatedPosition, 0.0, 1.0);
    vertexOut.texCoord = texCoord;
}
