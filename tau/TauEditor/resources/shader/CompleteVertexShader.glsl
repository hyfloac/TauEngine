#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out VertexData 
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} vOut;

uniform mat4 compoundMatrix;
uniform mat4 projectionMatrix;
uniform mat4 cameraViewMatrix;
uniform mat4 modelViewMatrix;

void main(void)
{
    vec4 pos4D = vec4(position, 1.0);

    gl_Position = projMatrix * cameraViewMatrix * modelViewMatrix * pos4D;

    fPos = vec3(model * pos4D);
    fNormal = normalize(normal);
    fTexCoords = texCoord;
}
