#version 330 core
#glsl vertex

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture;

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexture;

uniform mat4 projectionMatrx;
uniform mat4 cameraViewMatrix;
uniform mat4 modelViewMatrix;

void main(void)
{
    gl_Position = projectionMatrix * cameraViewMatrix * modelViewMatrix * vec4(position, 1.0);

    fPosition = position;
    fNormal = normal;
    fTexture = texture;
}
