#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture;

out vec3 color;
out vec2 fTexCoord;
out vec3 fNormal;

uniform mat4 compoundMatrix;
uniform mat4 projectionMatrix;
uniform mat4 cameraViewMatrix;
uniform mat4 modelViewMatrix;

void main(void)
{
    // mat4 _a = projectionMatrix * cameraViewMatrix;
    // mat4 _b = _a * modelViewMatrix;
    // gl_Position = _b * vec4(position, 1.0);

    gl_Position = ((projectionMatrix * cameraViewMatrix) * modelViewMatrix) * vec4(position, 1.0);
    // gl_Position = (projectionMatrix * (cameraViewMatrix * modelViewMatrix)) * vec4(position, 1.0);
    // gl_Position = compoundMatrix * modelViewMatrix * vec4(position, 1.0);
    // gl_Position = compoundMatrix * vec4(position, 1.0);

    color = normal;
    fTexCoord = texture;
    fNormal = normalize(normal);
}
