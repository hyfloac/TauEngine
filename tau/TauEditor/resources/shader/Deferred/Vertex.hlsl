struct VSInput
{
    float3 position : Position;
    float3 normal : Normal;
    float3 tangent : Tangent;
    float2 texCoord : TexCoord;
};

struct PSInput
{
    float4 svPosition : SV_Position;
    float3 position : Position;
    float2 texCoord : TexCoord;
    float3x3 tbn : TBN;
};

cbuffer CameraDynamicMatrices : register(b0)
{
    float4x4 mvp;
    float4x4 model;
};

cbuffer CameraStaticMatrices : register(b1)
{
    float4x4 projection;
    float4x4 view;
    float4 viewPos4;
}

PSInput vsMain(VSInput input)
{
    float4 pos = float4(input.position, 1.0f);
    pos = mul(model, pos);
    float4 svPos = pos;
    svPos = mul(view, svPos);
    svPos = mul(projection, svPos);

    float3 T = normalize(mul(model, float4(input.tangent, 0.0f)).xyz);
    float3 N = normalize(mul(model, float4(input.normal,  0.0f)).xyz);
    T = normalize(T - dot(T, N) * N);
    float3 B = cross(N, T);

    float3x3 TBN = float3x3(T, B, N);

    PSInput output;

    output.svPosition = svPos;
    output.position = pos.xyz;
    output.texCoord = input.texCoord;
    output.tbn = TBN;

    return output;
}
