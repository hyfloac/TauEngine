#version 430 core

layout(location = 0) in vec2 fTexCoord;

layout(location = 0) out vec4 fragColor;

#include <|TERes/shader/include/PointLight.glsl>
#include <|TERes/shader/include/SpotLight.glsl>

layout(std140, binding = 0) uniform PointLightUni
{
    PointLight pointLight;
};

layout(std140, binding = 1) uniform SpotLightUni
{
    SpotLight spotLight;
};

layout(std140, binding = 1) uniform CameraStaticMatrices
{
    mat4 projectionMatrix;
    mat4 cameraViewMatrix;
    vec4 viewPos4;
};

layout(location = 0) uniform sampler2D colorTexture;
layout(location = 1) uniform sampler2D positionTexture;
layout(location = 2) uniform sampler2D normalTexture;
layout(location = 3) uniform sampler2D specularTexture;
layout(location = 4) uniform sampler2D depthTexture;

void main(void)
{
    vec3 viewPos = viewPos4.xyz;

    vec3 diffuseTexel = vec3(texture(colorTexture, fTexCoord));
    vec4 positionTexel = texture(positionTexture, fTexCoord);
    vec3 normalTexel = vec3(texture(normalTexture, fTexCoord));
    vec3 specularTexel = vec3(texture(specularTexture, fTexCoord));
    gl_FragDepth = texture(depthTexture, fTexCoord).r;

    vec3 point = computePointLight(pointLight, viewPos, positionTexel.xyz, normalTexel, diffuseTexel, specularTexel, positionTexel.w);
    vec3 spot  =  computeSpotLight(spotLight,  viewPos, positionTexel.xyz, normalTexel, diffuseTexel, specularTexel, positionTexel.w);

    vec3 result = point + spot;

    fragColor = vec4(result, 1.0);
}
