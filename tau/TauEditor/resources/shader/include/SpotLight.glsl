struct SpotLight
{
    vec4 position;
    vec4 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
};

vec3 computeSpotLight(SpotLight light, vec3 cameraPos, vec3 pos, vec3 normal, vec3 diffuseSample, vec3 specularSample, float specExp)
{
    vec3 lightPosDelta = light.position.xyz - pos;
    vec3 lightDir = normalize(lightPosDelta);

    float theta = dot(lightDir, normalize(-light.direction.xyz));

    if(theta > light.outerCutOff)
    {
        float epsilon = light.cutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

        float distance = length(lightPosDelta);
        float attenuation = 1.0 / (light.constant + 
                                   light.linear    * distance + 
                                   light.quadratic * distance * distance);

        // vec3 norm = normalize(normal);

        vec3 viewDir = normalize(cameraPos - pos);
        vec3 reflectDir = reflect(-lightDir, normal);

        float diff = max(dot(normal, lightDir), 0.0);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), specExp);

        vec3 ambient  = light.ambient.xyz  *        diffuseSample;
        vec3 diffuse  = light.diffuse.xyz  * diff * diffuseSample  * attenuation * intensity;
        vec3 specular = light.specular.xyz * spec * specularSample * attenuation * intensity;

        return ambient + diffuse + specular;
    }
    else
    {
        return light.ambient.xyz * diffuseSample;
    }
}

vec3 computeSpotLightDebug(SpotLight light, vec3 cameraPos, vec3 pos, vec3 normal, vec3 diffuseSample, vec3 specularSample, float specExp)
{
    vec3 lightPosDelta = light.position.xyz - pos;
    vec3 lightDir = normalize(lightPosDelta);

    float theta = dot(lightDir, normalize(-light.direction.xyz));

    if(theta > light.outerCutOff)
    {
        float epsilon = light.cutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

        float distance = length(lightPosDelta);
        float attenuation = 1.0 / (light.constant + 
                                   light.linear    * distance + 
                                   light.quadratic * distance * distance);

        // vec3 norm = normalize(normal);

        vec3 viewDir = normalize(cameraPos - pos);
        vec3 reflectDir = reflect(-lightDir, normal);

        float diff = max(dot(normal, lightDir), 0.0);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), specExp);

        // vec3 ambient  = light.ambient.xyz  *        diffuseSample;
        vec3 ambient = vec3(0.0f);
        vec3 diffuse  = light.diffuse.xyz  * diff * diffuseSample  * attenuation * intensity;
        vec3 specular = light.specular.xyz * spec * specularSample * attenuation * intensity;

        return ambient + diffuse + specular;
    }
    else
    {
        // return light.ambient.xyz * diffuseSample;
        return vec3(0.0f);
    }
}
