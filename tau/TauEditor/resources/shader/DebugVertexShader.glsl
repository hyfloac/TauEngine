#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out VertexData 
{
    vec3 position;
    vec3 color;
    vec2 texCoord;
} vOut;

uniform mat4 compoundMatrix;
uniform mat4 projectionMatrix;
uniform mat4 cameraViewMatrix;
uniform mat4 modelViewMatrix;

float _Seed;

float rand()
{
    float result = fract(sin(_Seed / 100.0f * dot(vec2(0.5, 0.5), vec2(12.9898, 78.233))) * 43758.5453);
    _Seed += 1.0f;
    return result;
}

void main(void)
{
    vec4 pos4D = vec4(position, 1.0);

    gl_Position = projectionMatrix * cameraViewMatrix * modelViewMatrix * pos4D;

    _Seed = gl_VertexID;

    vOut.position = pos4D;
    vOut.color = vec4(rand(), rand(), rand());
    vOut.texCoord = texCoord;
}
