#version 330 core

in vec3 texCoord;

layout(location = 0) out vec4 fragColor;

uniform samplerCube skybox;

void main(void)
{
    fragColor = texture(skybox, texCoord);
}
