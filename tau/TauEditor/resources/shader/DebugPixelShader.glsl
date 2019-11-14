#version 330 core

in VertexData 
{
    vec3 position;
    vec3 color;
    vec2 texCoord;
} vIn;

layout(location = 0) out vec4 fragColor;

uniform sampler2D textureSampler;
uniform sampler2D textureOverlay;

void main(void)
{
    float z = gl_FragCoord.z / gl_FragCoord.w;
    z *= 0.25;
    z = 1.0 - z;
    
    vec4 texColor = texture(textureSampler, vIn.texCoord);
    vec4 overlayColor = texture(textureOverlay, vIn.texCoord);
    vec4 mixed = mix(texColor, overlayColor, 0.3);

    mixed = mixed * vec4(z, z, z, 1.0f);

    fragColor = mixed;
}
