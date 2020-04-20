struct PSInput
{
    float4 svPosition : SV_Position;
    float2 texCoord : TexCoord;
};

struct PSOutput
{
    float4 color : SV_Target0;
    float depth : SV_Depth;
};

cbuffer Saturation : register(b0)
{
    float saturation;
}

Texture2D<float4> colorTexture : register(t0);
Texture2D<float> depthTexture : register(t1);
SamplerState textureSampler : register(s0);

float4 vibrancyPP(float4 texel);

PSOutput psMain(PSInput input)
{
    float4 fbTexel = colorTexture.Sample(textureSampler, input.texCoord);
    float depthTexel = depthTexture.Sample(textureSampler, input.texCoord);

    PSOutput output;
    output.color = vibrancyPP(fbTexel);
    output.depth = clamp(depthTexel, -1.0f, 0.999999f);

    return output;
}

float3 rgb2hsv(float3 c);
float3 hsv2rgb(float3 c);

float4 vibrancyPP(float4 texel)
{
    float3 hsv = rgb2hsv(texel.rgb);
    hsv.g = saturate(hsv.g * saturation);
    return float4(hsv2rgb(hsv), 1.0f);
}

float3 rgb2hsv(float3 c)
{
    float4 K = float4(0.0f, -1.0f / 3.0f, 2.0f / 3.0f, -1.0f);
    float4 p = lerp(float4(c.bg, K.wz), float4(c.gb, K.xy), step(c.b, c.g));
    float4 q = lerp(float4(p.xyw, c.r), float4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10f;
    return float3(abs(q.z + (q.w - q.y) / (6.0f * d + e)), d / (q.x + e), q.x);
}

float3 hsv2rgb(float3 c)
{
    float4 K = float4(1.0f, 2.0f / 3.0f, 1.0f / 3.0f, 3.0f);
    float3 p = abs(frac(c.xxx + K.xyz) * 6.0f - K.www);
    return c.z * lerp(K.xxx, clamp(p - K.xxx, 0.0f, 1.0f), c.y);
}

