struct VSInput
{
    float3 position : Position;
    float3 normal : Normal;
    float3 tangent : Tangent
    float2 texCoord : TexCoord;
};

struct PSInput
{
    float4 svPosition : SV_Position;
    float3 position : Position;
    float2 texCoord : TexCoord;
    float3x3 TBN : TBN;
};

cbuffer Matrices : register(b0)
{
    float4x4 compound;
    float4x4 modelCubeMap;
}

PSInput vsMain(VSInput input)
{
    float4 pos4 = float4(input.position, 1.0f);

    float3 pos = mul(modelCubeMap, pos4).xyz;
    float4 svPos = mul(compound, pos4);

    float3 T = normalize(mul(model, float4(input.tangent, 0.0f)).xyz);
    float3 N = normalize(mul(model, float4(input.normal,  0.0f)).xyz);
    T = normalize(T - dot(T, N) * N);
    float3 B = cross(N, T);

    float3x3 TBN = float3x3(T, B, N);

    PSInput output;
    output.svPosition = svPos;
    output.position = pos;
    output.texCoord = input.texCoord;
    output.TBN = TBN;

    return output;
}
