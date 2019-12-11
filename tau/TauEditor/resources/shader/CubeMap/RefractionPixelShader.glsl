#version 330 core

in vec3 fPosition;
in vec3 fNormal;

out vec4 fragColor;

uniform vec3 cameraPos;
uniform samplerCube cubeMap;

const float refractionRatio = 1.0 / 1.52;

void main(void)
{
    vec3 I = normalize(fPosition - cameraPos);
    vec3 R = refract(I, normalize(fNormal), refractionRatio);
    fragColor = vec4(texture(cubeMap, R).rgb, 1.0);
}
