// https://john-chapman-graphics.blogspot.com/2013/02/pseudo-lens-flare.html
// https://www.shadertoy.com/view/XdfXRX
// 존 챔피언, iceecool 님들의 코드 기반으로 수정
#include "Common.hlsli"
#define HALF 0.5f


Texture2D iSceneTex : register(t0);
Texture2D iNoise : register(t1);
Texture2D iDepthTex : register(t2);
SamplerState iSampler : register(s0);


cbuffer LenFlareBuffer : register(b3)
{
    // Row 1: 기본 고스트 제어
    int iGhostCount; // 고스트 개수
    float iGhostSpacing; // 고스트 간격
    float iGhostThreshold; // 밝기 임계값
    float iGhostAlpha; // 전체 투명도

    // Row 2: 태양 위치 및 기본 글로우
    float2 iSunUV; // 태양의 Screen UV
    float iGlowSize; // 태양 주변 글로우 크기
    float iStarScale;

    // Row 3: 고스트 물리 속성 및 태양 코어
    float iGhostPull;
    float iGhostIntensity;
    float iGhostFalloff;
    float iSunCoreTight;

    // Row 4: 왜곡 및 휘도
    float3 iDistortion;
    float iPadding1;

    // Row 5: 휘도 기준
    float3 iLuminance;
    float iPadding2;

    // Row 6: F2 설정
    float3 iF2Offset;
    float iF2Sharpness;

    // Row 7: F2 색상
    float3 iF2ColorMult;
    float iPadding3;

    // Row 8: F4 설정
    float3 iF4Offset;
    float iF4Power;

    // Row 9: F4 색상
    float3 iF4ColorMult;
    float iPadding4;

    // Row 10: F5 설정
    float3 iF5Offset;
    float iF5Power;

    // Row 11: F5 색상
    float3 iF5ColorMult;
    float iPadding5;

    // Row 12: F6 설정
    float3 iF6Offset;
    float iF6Power;

    // Row 13: F6 색상
    float3 iF6ColorMult;
    float iPadding6;

    // Row 14~17: 행렬
    float4x4 iLensMatrix;
}; // LenFlareBuffer


float noise(float t)
{
    return iNoise.Sample(iSampler, float2(t, 0.0f)).x;
} // noise


float noise(float2 t)
{
    return iNoise.Sample(iSampler, t + float2(iTime * 0.1f, iTime * 0.1f)).x;
} // noise


// 뎁스 테스트용
float4 testDepth()
{
    
    float sunDepth = iDepthTex.Sample(iSampler, iSunUV).r;
    if (sunDepth > 0.9f)
        return float4(1, 0, 0, 1);
    else
        return float4(0, 0, 1, 1);
} // testDepth


float getLuminance(float3 color)
{
    return dot(color, iLuminance);
} // getLuminance


// 태양 주변 5점 샘플링 및 Threshold 적용
float calculateSunVisibility()
{
    // 텍스처 좌표 기준 오프셋
    float2 blurOffset = float2(0.003f, 0.003f * iResolution.x/iResolution.y);
    
    float3 samples[5];
    samples[0] = iSceneTex.Sample(iSampler, iSunUV).rgb;
    samples[1] = iSceneTex.Sample(iSampler, iSunUV + float2(blurOffset.x, 0)).rgb;
    samples[2] = iSceneTex.Sample(iSampler, iSunUV - float2(blurOffset.x, 0)).rgb;
    samples[3] = iSceneTex.Sample(iSampler, iSunUV + float2(0, blurOffset.y)).rgb;
    samples[4] = iSceneTex.Sample(iSampler, iSunUV - float2(0, blurOffset.y)).rgb;

    float combinedLuma = 0;
    
    [unroll]
    for (int i = 0; i < 5; i++)
    {
        float luma = getLuminance(samples[i]);
        
        // Threshold 적용
        luma = saturate((luma - iGhostThreshold) / (1.0f - iGhostThreshold));
        
        combinedLuma += luma;
    }
    
    return combinedLuma / 5.0f; // 5점 평균 반환
} // calculateSunVisibility


// 가시성 판단
float checkVisibility(float2 center)
{
    float avgLuma = calculateSunVisibility();
    float visibility = smoothstep(0.1f, 0.9f, avgLuma);

    // 화면 외곽 페이드 아웃
    float d = distance(iSunUV, center);
    float edgeFade = 1.0f - smoothstep(0.45f, 0.75f, d);

    return visibility * edgeFade * iGhostAlpha;
} // checkVisibility


// 광원 정중앙에 고정된 빛 갈라짐 효과
float3 coreGlow(float2 uv)
{
    float2 starOffset = iSunUV - uv;
    starOffset.x /= (iResolution.x/iResolution.y);
    float starSize = iGlowSize * iStarScale;
    float2 starUV = starOffset / starSize + HALF;
    
    float2 main = uv - iSunUV;
    
    float dist = length(main);
    float ang = atan2(main.y, main.x);
    
    // f0 기본 글로우
    float f0 = 1.0f / (dist * iSunCoreTight + 1.0f);
    
    // 무작위 빛 갈라짐 효과
    float n = noise(float2((ang - iTime / 9.0f) * 16.0f, pow(dist, 0.1f) * 32.0f));
    f0 = f0 + f0 * (sin((ang + iTime / 18.0f + noise(abs(ang) + n / 2.0f) * 2.0f) * 12.0f) * 0.1f + pow(dist, 0.1f) * 0.1f + 0.8f);

    float3(f0, f0, f0) * iStarScale;

    return float3(f0, f0, f0) * iStarScale;
} // coreGlow


// f2
float3 softHalo(float2 uvd, float2 pos)
{
    float3 color;
    
    color.r = max(1.0 / (1.0 + iF2Sharpness * pow(length(uvd + iF2Offset.r * pos), 2.0)), 0.0) * iF2ColorMult.r;
    color.g = max(1.0 / (1.0 + iF2Sharpness * pow(length(uvd + iF2Offset.g * pos), 2.0)), 0.0) * iF2ColorMult.g;
    color.b = max(1.0 / (1.0 + iF2Sharpness * pow(length(uvd + iF2Offset.b * pos), 2.0)), 0.0) * iF2ColorMult.b;
    return color;
} // softHalo


// f4, f5, f6
float3 spotsPattern(float2 uvx, float2 pos, float3 offsets, float power, float3 intensity)
{
    float3 color;
    
    color.r = max(0.01 - pow(length(uvx + offsets.r * pos), power), 0.0) * intensity.r;
    color.g = max(0.01 - pow(length(uvx + offsets.g * pos), power), 0.0) * intensity.g;
    color.b = max(0.01 - pow(length(uvx + offsets.b * pos), power), 0.0) * intensity.b;
    return color;
} // spotsPattern


float3 processLensFeatures(float2 uv, float2 center, float2 ghostVec, float2 direction, float3 distortion)
{
    float3 totalColor = float3(0.0f, 0.0f, 0.0f);
    
    float2 uv_center = uv - 0.5f;
    float2 pos_center = iSunUV - 0.5f;

    // 3D 상의 렌즈 움직임을 2D 평면에 투영
    uv_center = mul(float3(uv_center, 1.0f), (float3x3) iLensMatrix).xy;
    
    float2 uvd = uv_center * length(uv_center);
    float2 uvx_5 = lerp(uv_center, uvd, -0.5f); // 4
    float2 uvx_4 = lerp(uv_center, uvd, -0.4f); // 5

    [unroll(10)]
    for (int i = 0; i < iGhostCount; i++)
    {
        float2 loopOffset = ghostVec * (float) i * 0.1f;
        float2 currentPos = pos_center + loopOffset;

        float falloff = 1.0f - ((float) i / (float) iGhostCount);
        float weight = 1.0f - smoothstep(0.0f, 0.85f, length(currentPos));
        
        float intensityScale = iGlowSize * 1.2f; // 전체 밝기 조절용

        float3 element = float3(0, 0, 0);
        int pattern = i % 4;

        if (pattern == 0)
        {
            element = softHalo(uvd, currentPos);
        }
        else if (pattern == 1)
        {
            element = spotsPattern(uvx_5, currentPos, iF4Offset, iF4Power, iF4ColorMult);
        }
        else if (pattern == 2)
        {
            element = spotsPattern(uvx_4, currentPos, iF5Offset, iF5Power, iF5ColorMult);
        }
        else
        {
            element = spotsPattern(uvx_5, currentPos, iF6Offset, iF6Power, iF6ColorMult);
        }

        // 거리 감쇄 및 합성
        totalColor += element * weight * pow(falloff, iGhostFalloff) * intensityScale;
    }

    return totalColor;
} // processLensFeatures


float4 main(PixelInput input) : SV_TARGET
{
    float2 uv = input.tex;
    float2 center = float2(HALF, HALF);

    float finalAlpha = checkVisibility(center);

    if (finalAlpha <= 0.0001f)
        return float4(0, 0, 0, 0);

    float2 ghostVec = (center - iSunUV) * iGhostSpacing;
    float2 direction = normalize(ghostVec);
    float3 distortion = iDistortion;
    
    float3 finalColor = float3(0, 0, 0);

    finalColor += coreGlow(uv);
    finalColor += processLensFeatures(uv, center, ghostVec, direction, distortion);

    return float4(finalColor * finalAlpha, 1.0f);
} // main