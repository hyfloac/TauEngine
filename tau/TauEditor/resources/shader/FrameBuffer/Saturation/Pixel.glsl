#version 430 core

in vec2 fTexCoord;

layout(location = 0) out vec4 fragColor;

#include <|TERes/shader/include/Common.glsl>
#include <|TERes/shader/include/HSV.glsl>

layout(std140, binding = 0) uniform Saturation
{
    float saturation;
};

layout(location = 0) uniform sampler2D colorTexture;
layout(location = 1) uniform sampler2D depthTexture;

vec4 vibrancyPP(vec3 color);

void main(void)
{
    vec3 color = texture(colorTexture, fTexCoord).rgb;
    gl_FragDepth = clamp(texture(depthTexture, fTexCoord).r, -1.0, 0.999999);
    fragColor = vibrancyPP(color);
}

vec4 vibrancyPP(vec3 color)
{
    vec3 hsv = rgb2hsv(color);
    hsv.g = saturate(hsv.g * saturation);
    return vec4(hsv2rgb(hsv), 1.0);
}
