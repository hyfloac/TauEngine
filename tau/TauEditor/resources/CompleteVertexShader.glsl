#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec3 fPos;
out vec3 fNormal;
out vec2 fTexCoords;

uniform mat4 projMatrix;
uniform mat4 cameraViewMatrix;
uniform mat4 modelViewMatrix;

void main(void)
{
    vec4 pos4D = vec4(position, 1.0);

    gl_Position = projMatrix * cameraViewMatrix * modelViewMatrix * pos4D;

    fPos = vec3(model * pos4D);
    fNormal = normalize(normal);
    fTexCoords = texCoords;
}
