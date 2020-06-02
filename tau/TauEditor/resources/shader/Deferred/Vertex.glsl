#version 420 core

out gl_PerVertex
{
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 texCoord;

layout(location = 0) out vec3 fPosition;
layout(location = 1) out vec2 fTexCoord;
layout(location = 2) out mat3 fTBN;

layout(std140, binding = 0) uniform CameraDynamicMatrices
{
    mat4 mvpMatrix;
    mat4 modelViewMatrix;
};

layout(std140, binding = 1) uniform CameraStaticMatrices
{
    mat4 projectionMatrix;
    mat4 cameraViewMatrix;
    vec4 cameraPos;
};

void main(void)
{
    vec4 pos4 = vec4(position, 1.0);
    gl_Position = projectionMatrix * cameraViewMatrix * modelViewMatrix * pos4;

    fPosition = vec3(modelViewMatrix * pos4);
    fTexCoord = texCoord;

    vec3 T = normalize(vec3(modelViewMatrix * vec4(tangent, 0.0)));
    vec3 N = normalize(vec3(modelViewMatrix * vec4(normal, 0.0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    fTBN = mat3(T, B, N);
}
