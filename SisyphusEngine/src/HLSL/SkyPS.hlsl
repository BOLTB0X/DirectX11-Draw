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