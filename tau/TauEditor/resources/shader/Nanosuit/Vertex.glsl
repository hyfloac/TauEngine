#version 330 core

in vec3 position;
in vec3 normal;
in vec3 tangent;
in vec2 texCoord;

out vec3 fPosition;
out vec2 fTexCoord;
out mat3 fTBN;

layout(std140) uniform CameraMatrices
{
    mat4 proj;
    mat4 camera;
    mat4 model;
};

void main(void)
{
    vec4 pos4 = vec4(position, 1.0);
    gl_Position = proj * camera * model * pos4;

    fPosition = vec3(model * pos4);
    fTexCoord = texCoord;

    vec3 T = normalize(vec3(model * vec4(tangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(normal, 0.0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    fTBN = mat3(T, B, N);
}

