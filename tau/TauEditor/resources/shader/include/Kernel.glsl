vec4 kernelCompute(sampler2D sampler, vec2 texCoord, float kernel[9])
{
    const float offset = 1.0 / 300.0; 
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    vec3 color = vec3(0.0);
    for(int i = 0; i < 9; ++i)
    {
        vec3 texSample = vec3(texture(sampler, texCoord.st + offsets[i]));
        color += texSample * kernel[i];
    }

    return vec4(color, 1.0);
}

vec4 kernelTest(sampler2D sampler, vec2 texCoord)
{
    float kernel[9] = float[](
        2,   2, 2,
        2, -15, 2,
        2,   2, 2
    );

    return kernelCompute(sampler, texCoord, kernel);
}

vec4 kernelSharpen(sampler2D sampler, vec2 texCoord)
{
    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    return kernelCompute(sampler, texCoord, kernel);
}

vec4 kernelBlur(sampler2D sampler, vec2 texCoord)
{
    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );

    return kernelCompute(sampler, texCoord, kernel);
}

vec4 kernelEdgeDetect(sampler2D sampler, vec2 texCoord)
{
    float kernel[9] = float[](
        1,  1, 1,
        1, -8, 1,
        1,  1, 1
    );

    return kernelCompute(sampler, texCoord, kernel);
}
