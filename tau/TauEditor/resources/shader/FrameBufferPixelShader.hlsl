struct PSInput
{
    float4 svPosition : SV_Position;
    float2 texCoord : TexCoord;
};

Texture2D<float4> frameBuffer : register(t0);
SamplerState textureSampler;

float4 vibrancyPP(float4 texel);

float4 psMain(PSInput input) : SV_TARGET
{
    float4 fbTexel = frameBuffer.Sample(textureSampler, input.texCoord);

    float4 vibrantTexel = vibrancyPP(fbTexel);

    return vibrantTexel;
}

float3 rgb2hsv(float3 c);
float3 hsv2rgb(float3 c);

float4 vibrancyPP(float4 texel)
{
    float3 hsv = rgb2hsv(texel.rgb);
    hsv.g = saturate(hsv.g * 1.5f);
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

