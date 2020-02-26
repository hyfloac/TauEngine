#version 430 core

in vec3 texCoord;

layout(location = 0) out vec4 fragColor;

layout(location = 0) uniform samplerCube skybox;

// uniform samplerCube skybox;

void main(void)
{
    fragColor = texture(skybox, texCoord);
}
