#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 bitangent;
layout(location = 4) in vec2 texCoord;

out vec3 fPosition;
out vec3 fNormal;
out vec3 fTangent;
out vec3 fBiTangent;
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
    vec4 pos4 = vec4(position, 1.0);
    gl_Position = projectionMatrix * cameraViewMatrix * modelViewMatrix * pos4;

    // vertexOut.position = position;
    // vertexOut.normal = normalize(normal);
    // vertexOut.texCoord = texCoord;
    fPosition = vec3(modelViewMatrix * pos4);
    vec3 norm = mat3(transpose(inverse(modelViewMatrix))) * normalize(normal);
    fNormal = normalize(norm);
    fTangent = normalize(fTangent);
    fBiTangent = normalize(fBiTangent);
    fTexCoord = texCoord;
}
