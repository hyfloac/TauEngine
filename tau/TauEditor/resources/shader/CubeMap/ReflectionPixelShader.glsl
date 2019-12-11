#version 330 core

in vec3 fPosition;
in vec3 fNormal;

out vec4 fragColor;

uniform vec3 cameraPos;
uniform samplerCube cubeMap;

void main(void)
{
    vec3 I = normalize(fPosition - cameraPos);
    vec3 R = reflect(I, normalize(fNormal));
    fragColor = vec4(texture(cubeMap, R).rgb, 1.0);
}
