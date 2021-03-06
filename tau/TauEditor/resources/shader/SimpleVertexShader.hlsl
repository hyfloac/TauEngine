struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
};

struct PSInput
{
    float4 svPosition : SV_POSITION;
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
    float3x3 tbn : TBN;
};

cbuffer Matrices : register(b0)
{
    // float4x4 compound;
    float4x4 projection;
    float4x4 view;
    float4x4 model;
};

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
    // float3x3 TBN = float3x3(N, B, T);

    PSInput output;

    output.svPosition = svPos;
    output.position = pos.xyz;
    output.texCoord = input.texCoord;
    output.tbn = TBN;

    return output;
}
