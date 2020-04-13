#version 430 core

in vec2 fTexCoord;

layout(location = 0) out vec4 fragColor;

layout(location = 0) uniform sampler2D frameBufferSampler;

// uniform sampler2D frameBufferSampler;

#include <|TERes/shader/include/Branchless.glsl>
#include <|TERes/shader/include/Common.glsl>
#include <|TERes/shader/include/Kernel.glsl>
#include <|TERes/shader/include/Grayscale.glsl>
#include <|TERes/shader/include/HSV.glsl>

const float luminanceThreshold = 0.2;
const float colorAmplification = 4.0;
const vec3 lumDot = vec3(0.3, 0.59, 0.11);
const vec3 visionColor = vec3(0.1, 0.95, 0.2);

vec3 nightVision(sampler2D sampler, vec2 texCoord)
{
    float noise = rand(texCoord) * 0.1;
    vec4 color = texture(sampler, texCoord);
    vec3 color3 = color.rgb;

    float lum = dot(lumDot, color3);
    float when = when_lt(lum, luminanceThreshold);
    color3 *= colorAmplification * when + not(when);

    vec3 finalColor = color3 + noise;
    finalColor *= visionColor;

    return finalColor * visionColor;
}

vec4 postProcess(sampler2D sampler, vec2 texCoord)
{
#if 0
    vec4 edge = kernelEdgeDetect(sampler, texCoord);
    vec4 grayEdge = grayscaleWeighted(edge) * 0.4;
    vec3 invEdge = 1.0 - grayEdge.rgb;
    // invEdge *= when_ge(gl_FragCoord.x, 400);
    // invEdge += 1.0 * when_eq(invEdge, vec3(0.0));
    return vec4(nightVision(sampler, texCoord) * invEdge, 1.0);
#else
    // return kernelEdgeDetect(sampler, texCoord);
    return kernelTest(sampler, texCoord);
#endif
}

vec4 vibrancyPP(sampler2D sampler, vec2 texCoord)
{
    vec3 color = texture(sampler, texCoord).rgb;
    vec3 hsv = rgb2hsv(color);
    hsv.g = saturate(hsv.g * 1.5);
    return vec4(hsv2rgb(hsv), 1.0);
}

void main(void)
{
    // fragColor = postProcess(frameBufferSampler, fTexCoord);
    fragColor = vibrancyPP(frameBufferSampler, fTexCoord);
    // fragColor = texture(frameBufferSampler, fTexCoord);
    // fragColor = vec4(nightVision(frameBufferSampler, fTexCoord), 1.0);
}
