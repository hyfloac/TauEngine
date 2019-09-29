#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture;

out vec4 color;
out vec2 fTexCoord;

uniform mat4 compoundMatrix;
uniform mat4 projectionMatrix;
uniform mat4 cameraViewMatrix;
uniform mat4 modelViewMatrix;

int s_seed;

int fastRand(void)
{
    s_seed = 214013 * s_seed + 2531011;
    return s_seed;
}

float _Seed;

float rand()
{
    float result = fract(sin(_Seed / 100.0f * dot(vec2(0.5, 0.5), vec2(12.9898, 78.233))) * 43758.5453);
    _Seed += 1.0f;
    return result;
}

void main(void)
{
    fTexCoord = texture;

    gl_Position = projectionMatrix * cameraViewMatrix * modelViewMatrix * vec4(position, 1.0);
    // gl_Position = compoundMatrix * vec4(position, 1.0);

    s_seed = gl_VertexID;
    _Seed = gl_VertexID;

    // int r = fastRand() % 256;
    // int g = fastRand() % 256;
    // int b = fastRand() % 256;

    // color = vec4(r / 256.0, g / 256.0, b / 256.0, 1.0);
    color = vec4(rand(), rand(), rand(), 1.0);
}
