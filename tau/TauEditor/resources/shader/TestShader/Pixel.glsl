#version 420 core 

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;

layout(location = 0) out vec4 fragColor;

void main(void)
{
    fragColor = vec4((fPosition / 2.0) + 1.0, 1.0);
}
