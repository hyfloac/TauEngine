struct PointLight
{
    vec4 position;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float constant;
    float linear;
    float quadratic;
};

vec3 computePointLight(PointLight light, vec3 cameraPos, vec3 pos, vec3 normal, vec3 diffuseSample, vec3 specularSample, float specExp)
{
    vec3 lightPosDelta = light.position.xyz - pos;

    float distance = length(lightPosDelta);
    float attenuation = 1.0 / (light.constant + 
                               light.linear    * distance + 
                               light.quadratic * distance * distance);

    // vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosDelta);

    vec3 viewDir = normalize(cameraPos - pos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specExp);

    vec3 ambient  = light.ambient.xyz  *        diffuseSample;
    vec3 diffuse  = light.diffuse.xyz  * diff * diffuseSample  * attenuation;
    vec3 specular = light.specular.xyz * spec * specularSample * attenuation;

    return ambient + diffuse + specular;
}

vec3 computePointLightDebug(PointLight light, vec3 cameraPos, vec3 pos, vec3 normal, vec3 diffuseSample, vec3 specularSample, float specExp)
{
    vec3 lightPosDelta = light.position.xyz - pos;

    float distance = length(lightPosDelta);
    float attenuation = 1.0 / (light.constant + 
                               light.linear    * distance + 
                               light.quadratic * distance * distance);

    // vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosDelta);

    vec3 viewDir = normalize(cameraPos - pos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specExp);

    // vec3 ambient  = light.ambient.xyz  *        diffuseSample;
    vec3 ambient = vec3(0.0f);
    vec3 diffuse  = light.diffuse.xyz  * diff * diffuseSample  * attenuation;
    // vec3 diffuse = vec3(0.0f);
    vec3 specular = light.specular.xyz * spec * specularSample * attenuation;

    return ambient + diffuse + specular;
}
