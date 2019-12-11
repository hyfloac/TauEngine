#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 fPosition;
out vec3 fNormal;

uniform mat4 projectionMatrix;
uniform mat4 cameraViewMatrix;
uniform mat4 modelViewMatrix;

void main(void)
{
    fNormal = mat3(transpose(inverse(modelViewMatrix))) * normal;
    vec4 pos = vec4(position, 1.0);
    fPosition = vec3(modelViewMatrix * pos);
    gl_Position = projectionMatrix * cameraViewMatrix * modelViewMatrix * pos;
}
