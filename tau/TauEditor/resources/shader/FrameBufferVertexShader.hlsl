struct VSInput
{
    float2 position : Position;
    float2 texCoord : TexCoord;
};

struct PSInput
{
    float4 svPosition : SV_Position;
    float2 texCoord : TexCoord;
};

PSInput vsMain(VSInput input)
{
    PSInput output;
    output.svPosition = float4(input.position, 0.0f, 1.0f);
    output.texCoord = input.texCoord;
    output.texCoord.y = 1.0f - output.texCoord.y;
    return output;
}
