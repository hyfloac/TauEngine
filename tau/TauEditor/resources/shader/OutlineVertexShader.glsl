#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 projectionMatrix;
uniform mat4 cameraViewMatrix;
uniform mat4 modelViewMatrix;

uniform float scaleFactor;

void main(void)
{
    vec3 norm = normalize(normal);
    vec3 pos = position + norm * scaleFactor;

    gl_Position = projectionMatrix * cameraViewMatrix * modelViewMatrix * vec4(pos, 1.0);
}
