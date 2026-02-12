// https://www.shadertoy.com/view/4dl3zr, 배경과 태양부분 참고
#include "Common.hlsli"
#include "Maths.hlsli"

#define SKY_LOWER_COLOR float3(0.05f, 0.02f, 0.1f)
#define SUN_DIST_SCALE 0.2f
#define BLOOM_MULT 2.5f
#define GLOW_MULT 0.6f
#define RAY_FREQ 3.0f
#define RAY_TIME_SCALE 0.15f


struct PixelSkyInput
{
    float4 position : SV_POSITION;
    float3 localPos : TEXCOORD0;
}; // PixelSkyInput


cbuffer SkyBuffer : register(b3)
{
    // Row 1
    float3 iSkyTopColor;
    float sPadding1;

    // Row 2
    float3 iSkyHorizonColor;
    float iSkyExponent;

    // Row 3
    float iSunSize;
    float iSunBloom;
    float iSunIntensity;
    float iRayStrength;

    // Row 4
    float iRayAnimSpeed;
    float3 sPadding2;
}; // SkyBuffer


// 2D Simplex Noise 구현
float snoise(float2 v)
{
    const float4 C = float4(0.211324865405187f, // (3.0-sqrt(3.0))/6.0
                            0.366025403784439f, // 0.5*(sqrt(3.0)-1.0)
                           -0.577350269189626f, // -1.0 + 2.0 * C.x
                            0.024390243902439f); // 1.0 / 41.0

    // First corner
    float2 i = floor(v + dot(v, C.yy));
    float2 x0 = v - i + dot(i, C.xx);

    // Other corners
    float2 i1;
    i1 = (x0.x > x0.y) ? float2(1.0f, 0.0f) : float2(0.0f, 1.0f);
    float4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;

    // 순열
    i = mod289(i);
    float3 p = permute(permute(i.y + float3(0.0f, i1.y, 1.0f))
               + i.x + float3(0.0f, i1.x, 1.0f));

    float3 m = max(0.5f - float3(dot(x0, x0), dot(x12.xy, x12.xy), dot(x12.zw, x12.zw)), 0.0f);
    m = m * m;
    m = m * m;

    // 그레디언트
    float3 x = 2.0f * frac(p * C.www) - 1.0f;
    float3 h = abs(x) - 0.5f;
    float3 ox = floor(x + 0.5f);
    float3 a0 = x - ox;

    // 정규화 그레디언트
    m *= 1.79284291400159f - 0.85373472095314f * (a0 * a0 + h * h);

    float3 g;
    
    g.x = a0.x * x0.x + h.x * x0.y;
    g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    
    return 130.0f * dot(m, g);
} // snoise


// 태양과의 각도 거리 계산
float getSunDistance(float3 rd, float3 sd)
{
    float sunDot = dot(rd, sd);
    return acos(saturate(sunDot)) * SUN_DIST_SCALE;
} // getSunDistance


// 태양의 Core 및 Glow 색상 계산
float3 coreGlow(float sunDist, float3 lightColor)
{
    float core = smoothstep(iSunSize, iSunSize * 0.4f, sunDist);
    
    //  Bloom & Wide Glow
    float bloom = exp(-sunDist * iSunBloom) * BLOOM_MULT;
    float wideGlow = exp(-sunDist * (iSunBloom * 0.125f)) * GLOW_MULT;

    return lightColor * (core * iSunIntensity + bloom + wideGlow);
} // CoreGlow


// 배경이 되는 하늘의 그라데이션 계산
float3 skyBackground(float3 rd)
{
    float horizonFactor = pow(abs(rd.y), iSkyExponent);
    
    if (rd.y > 0)
        return lerp(iSkyHorizonColor, iSkyTopColor, horizonFactor);
    
    // 지평선 아래는 어두운 색상으로 처리
    return lerp(iSkyHorizonColor, SKY_LOWER_COLOR, horizonFactor);
} // skyBackground


float4 main(PixelSkyInput input) : SV_TARGET
{
    float3 rd = normalize(input.localPos);
    float3 sd = normalize(iLightDirection);
    
    float3 finalColor = skyBackground(rd);

    // 태양까지의 거리 계산
    float sunDist = getSunDistance(rd, sd);
    
    // 태양 스타일링 적용 및 강도 조절
    finalColor += coreGlow(sunDist, iLightColor.rgb) * iIntensity;

    return float4(finalColor, 0.8f);
} // main