struct VSInput
{
    float2 position : POSITION;
    float4 color : COLOR;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PSInput vsMain(VSInput input)
{
    PSInput output;

    output.position = float4(input.position, 0.0f, 1.0f);
    output.color = input.color;

    return output;
}
