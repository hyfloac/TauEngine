#version 330 core
#glsl fragment

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexture;

out vec4 fragColor;

struct Attenuation
{
    float constant;
    float linear;
    float exponent;
};

struct PointLight
{
    vec3 color;
    vec3 position;
    float intensity;
    Attenuation attenuation;
};

struct Material
{
    vec4 color;
    int useLight;
    sampler2D textureSampler;
    int useTexture;
    float reflectance;
};

uniform Matrial material;
uniform vec3 ambientLight;
uniform vec3 cameraPos;
uniform PointLight pointLight;
uniform float specularPower;

vec4 calcPointLight(PointLight light, vec3 position, vec3 normal)
{
    vec4 diffuseColor = vec4(0.0);
    vec4 specColor = vec4(0.0);

    vec3 lightDirection = light.position - position;
    vec3 toLightSource  = normalize(lightDirection);
    float diffuseFactor = max(dot(normal, toLightSource), 0.0);
    diffuseColor = vec4(light.color, 1.0) * light.intensity * diffuseFactor;

    vec3 cameraDirection = normalize(cameraPos - position);
    vec3 fromLightSource = -toLightSource;
    vec3 reflectedLight = normalize(reflect(fromLightSource, normal));
    float specularFactor = max(dot(cameraDirection, reflectedLight), 0.0);
    specularFactor = pow(specularFactor, specularPower);
    specColor = specularFactor * material.reflectance * vec4(light.color, 1.0);

    float dist = length(lightDirection);
    float attenuationInv = light.att.constant + light.att.linear * dist + light.att.exponent * dist * dist;
    return (diffuseColor + specColor) / attenuationInv;
}

void main(void)
{
    vec4 baseColor = material.color;

    vec4 textureColor = texture(material.textureSampler, fTexture);
    baseColor += textureColor * material.useTexture;

    vec4 antiLight = vec4(1.0 - useLight);
    vec4 totalLight = vec4(ambientLight, 1.0) * useLight + calcPointLight(pointLight, fPosition, fNormal) * useLight + antiLight;

    fragColor = baseColor * totalLight;
}
