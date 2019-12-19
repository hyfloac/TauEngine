#version 330 core

layout(location = 0) in vec3 position;

out vec3 texCoord;

uniform mat4 projectionMatrix;
uniform mat4 cameraViewMatrix;

void main(void)
{
    texCoord = position;
    vec4 pos = projectionMatrix * cameraViewMatrix * vec4(position, 1.0);
    gl_Position = pos.xyww;
}
