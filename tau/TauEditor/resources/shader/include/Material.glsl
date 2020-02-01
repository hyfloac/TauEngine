struct Material 
{
    float specularExponent;
};

struct MaterialTextures
{
    sampler2D diffuseSampler;
    sampler2D specularSampler;
    sampler2D normalSampler;
};
