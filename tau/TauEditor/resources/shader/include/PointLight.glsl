struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

vec3 computePointLight(PointLight light, vec3 cameraPos, vec3 pos, vec3 normal, vec3 diffuseSample, vec3 specularSample, float specExp)
{
    vec3 lightPosDelta = light.position - pos;

    float distance = length(lightPosDelta);
    float attenuation = 1.0 / (light.constant + 
                               light.linear    * distance + 
                               light.quadratic * distance * distance);

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosDelta);

    vec3 viewDir = normalize(cameraPos - pos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specExp);

    vec3 ambient  = light.ambient  *        diffuseSample;
    vec3 diffuse  = light.diffuse  * diff * diffuseSample  * attenuation;
    vec3 specular = light.specular * spec * specularSample * attenuation;

    return ambient + diffuse + specular;
}
