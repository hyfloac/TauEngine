#version 420 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 3) in vec2 texCoord;

out vec3 fPosition;
out vec3 fNormal;
out vec3 fTexCoord;

layout(std140, binding = 0) uniform CameraMatrices
{
    // mat4 compoundMatrix;
    mat4 projectionMatrix;
    mat4 cameraViewMatrix;
    mat4 modelViewMatrix;
};

void main(void)
{
    vec4 pos4 = vec4(position, 1.0);
    gl_Position = projectionMatrix * cameraViewMatrix * modelViewMatrix * pos4;

    fPosition = position;
    fNormal = normalize(normal);
    fTexCoord = fTexCoord;
}
