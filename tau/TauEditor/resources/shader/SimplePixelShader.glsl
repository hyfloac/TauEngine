#version 330 core

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;

// in VertexData 
// {
//     vec3 position;
//     vec3 color;
//     vec2 texCoord;
// } vertexIn;

layout(location = 0) out vec4 fragColor;

uniform sampler2D textureSampler;
uniform sampler2D textureOverlay;

void main(void)
{
    vec4 texColor = texture(textureSampler, fTexCoord);
    vec4 overlayColor = texture(textureOverlay, fTexCoord);
    fragColor = mix(texColor, overlayColor, 0.3);
}
