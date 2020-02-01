#version 420 core

layout(location = 0) in vec3 position;

out vec3 texCoord;

// uniform mat4 projectionMatrix;
// uniform mat4 cameraViewMatrix;

layout(binding = 0) uniform CameraMatrices
{
    mat4 projectionMatrix;
    mat4 cameraViewMatrix;
};

void main(void)
{
    texCoord = position;
    vec4 pos = projectionMatrix * cameraViewMatrix * vec4(position, 1.0);
    gl_Position = pos.xyww;
}
