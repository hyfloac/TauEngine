struct PSInput 
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 flatten(float4 color);
float4 flatten0(float4 color);

float4 psMain(PSInput input) : SV_TARGET
{
    float4 color = input.color;

    // color = flatten(color);

    return color;
}

float4 flatten(float4 color)
{
    if(color.r > color.g)
    {
        color.g = 0.0f;
    }
    else
    {
        color.r = 0.0f;
    }
    
    if(color.g > color.b)
    {
        color.b = 0.0f;
    }
    else
    {
        color.g = 0.0f;
    }

    if(color.b > color.r)
    {
        color.r = 0.0f;
    }
    else
    {
        color.b = 0.0f;
    }

    return color;
}

float4 flatten0(float4 color)
{
    if(color.r > color.g)
    {
        color.g = 0.0f;
    }
    
    if(color.g > color.b)
    {
        color.b = 0.0f;
    }

    if(color.b > color.r)
    {
        color.r = 0.0f;
    }

    return color;
}
