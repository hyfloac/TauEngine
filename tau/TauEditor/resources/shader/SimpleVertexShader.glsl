#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexCoord;

// out VertexData 
// {
//     vec3 position;
//     vec3 normal;
//     vec2 texCoord;
// } vertexOut;

uniform mat4 compoundMatrix;
uniform mat4 projectionMatrix;
uniform mat4 cameraViewMatrix;
uniform mat4 modelViewMatrix;

void main(void)
{
    gl_Position = projectionMatrix * cameraViewMatrix * modelViewMatrix * vec4(position, 1.0);

    // vertexOut.position = position;
    // vertexOut.normal = normalize(normal);
    // vertexOut.texCoord = texCoord;
    fPosition = position;
    fNormal = normalize(normal);
    fTexCoord = texCoord;
}
