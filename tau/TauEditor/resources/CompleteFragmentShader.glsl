#version 330 core

struct Material
{
    sampler2D diffuseTexture;
    sampler2D overlayTexture;
    sampler2D specularMap;
    sampler2D normalMap;
    float shininess;
};

struct DirectionalLight
{
    vec3 direction;

    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
};

#define POINT_LIGHTS 4

in vec3 fPos;
in vec3 fNormal;
in vec2 fTexCoords;

layout(location = 0) out vec4 fragColor;

uniform vec3 viewPos;

uniform DirectionalLight dirLight;
uniform PointLight pointLights[POINT_LIGHTS];
uniform SpotLight spotLight;

uniform Material material;

vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 diffusePixel, vec3 specularPixel)
{
    vec3 lightDir = normalize(-light.direction);

    float diffuse = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambientColor * diffusePixel;
    vec3 diffuse = light.diffuseColor * diff * diffuseColor;
    vec3 specular = light.specularColor * spec * specularPixel;

    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 diffusePixel, vec3 specularPixel)
{
    vec3 lightDir = normalize(light.position - fPos);

    float diffuse = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambientColor * diffusePixel * attenuation;
    vec3 diffuse = light.diffuseColor * diff * diffuseColor * attenuation;
    vec3 specular = light.specularColor * spec * specularPixel * attenuation;

    return ambient + diffuse + specular;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 diffusePixel, vec3 specularPixel)
{
    vec3 lightDir = normalize(light.position - fPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess)

    float distance = length(light.position - fPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambientColor * diffusePixel * attenuation;
    vec3 diffuse = light.diffuseColor * diff * diffuseColor * attenuation;
    vec3 specular = light.specularColor * spec * specularPixel * attenuation;

    return ambient + diffuse + specular;
}

void main(void)
{
    vec3 viewDir = normalize(viewPos - fPos);
    vec3 diffusePixel = vec3(texture(material.diffuseTexture, fTexCoords));
    vec3 specularPixel = vec3(texture(material.specularMap, fTexCoords));

    vec3 normal = texture(normalMap, fTexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    vec3 result = calcDirLight(dirLight, fNormal, viewDir, diffusePixel, specularPixel);

    for(int i = 0; i < POINT_LIGHTS; ++i)
    {
        result += calcPointLight(pointLights[i], fNormal, viewdir, diffusePixel, specularPixel);
    }

    result += calcSpotLight(spotLight, fNormal, viewdir, diffusePixel, specularPixel);

    fragColor = vec4(result, 1.0);
}
