#version 330 core

in vec4 color;
in vec2 fTexCoord;

layout(location = 0) out vec4 fragColor;

uniform sampler2D textureSampler;
uniform sampler2D textureOverlay;

float _Seed;

void main(void)
{
    //fragColor = color;

    float z = gl_FragCoord.z / gl_FragCoord.w;
    z *= 0.25;
    z = 1.0 - z;
    
    vec4 texColor = texture(textureSampler, fTexCoord);
    vec4 overlayColor = texture(textureOverlay, fTexCoord);
    vec4 mixed = mix(texColor, overlayColor, 0.3);

    mixed = mixed * vec4(z, z, z, 1.0f);

    fragColor = mixed;
}
