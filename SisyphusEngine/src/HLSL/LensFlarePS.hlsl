// https://john-chapman-graphics.blogspot.com/2013/02/pseudo-lens-flare.html
// https://www.shadertoy.com/view/XdfXRX
// 존 챔피언, iceecool 님들의 코드 기반으로 수정
#include "Common.hlsli"

// main에서 사용할 상수 오프셋과 배율
#define HALF 0.5f
#define DISTORTION float3(-0.005, 0.0, 0.005)
#define STAR_SCALE  0.8f
#define SUN_CORE_TIGHTNESS 36.0f  // 숫자가 클수록 태양 본체가 작아짐
#define LUMINANCE float3(0.3, 0.59, 0.11)

// 고스트 관련 오프셋과 배율
#define GHOST_PULL 0.1f // 고스트가 태양으로부터 떨어지는 간격 배율
#define GHOST_INTENSITY 1.2f // 전체 고스트 밝기 배율
#define GHOST_FALLOFF_POW 1.5f // 숫자가 클수록 멀리 있는 고스트가 빨리 흐려짐

// F2 오프셋과 배율
#define F2_SHARPNESS 32.0f
#define F2_OFFSET float3(0.80, 0.85, 0.90)
#define F2_COLOR_MULT float3(0.25, 0.23, 0.21)

// f4 오프셋과 배율
#define F4_POWER  2.4f
#define F4_OFFSET float3(0.40, 0.45, 0.50)
#define F4_COLOR_MULT float3(6.0, 5.0, 3.0)

// f5 오프셋과 배율
#define F5_POWER  5.5f
#define F5_OFFSET  float3(0.20, 0.40, 0.60)
#define F5_COLOR_MULT float3(2.0, 2.0, 2.0)

// f6 오프셋과 배율
#define F6_POWER 1.6f
#define F6_OFFSET float3(-0.3, -0.325, -0.35)
#define F6_COLOR_MULT float3(6.0, 3.0, 5.0)


Texture2D iSceneTex : register(t0);
Texture2D iNoise : register(t1);
Texture2D iDepthTex : register(t2);
SamplerState iSampler : register(s0);


cbuffer LenFlareBuffer : register(b3)
{
    int iGhostCount;
    float iGhostSpacing;
    float iGhostThreshold;
    float iGhostAlpha;
    
    float iAspect;
    float iGlowSize;
    float2 iSunUV;
    
    float4x4 iLensMatrix;
}; // GhostBuffer


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
    return dot(color, LUMINANCE);
} // getLuminance


// 태양 주변 5점 샘플링 및 Threshold 적용
float calculateSunVisibility()
{
    // 텍스처 좌표 기준 오프셋
    float2 blurOffset = float2(0.003f, 0.003f * iAspect);
    
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
    starOffset.x /= iAspect;
    float starSize = iGlowSize * STAR_SCALE;
    float2 starUV = starOffset / starSize + HALF;
    
    float2 main = uv - iSunUV;
    
    float dist = length(main);
    float ang = atan2(main.y, main.x);
    
    // f0 기본 글로우
    float f0 = 1.0f / (dist * SUN_CORE_TIGHTNESS + 1.0f);
    
    // 무작위 빛 갈라짐 효과
    float n = noise(float2((ang - iTime / 9.0f) * 16.0f, pow(dist, 0.1f) * 32.0f));
    f0 = f0 + f0 * (sin((ang + iTime / 18.0f + noise(abs(ang) + n / 2.0f) * 2.0f) * 12.0f) * 0.1f + pow(dist, 0.1f) * 0.1f + 0.8f);

    float3(f0, f0, f0) * STAR_SCALE;

    return float3(f0, f0, f0) * STAR_SCALE;
} // coreGlow


// f2
float3 softHalo(float2 uvd, float2 pos)
{
    float3 color;
    
    color.r = max(1.0 / (1.0 + F2_SHARPNESS * pow(length(uvd + F2_OFFSET.r * pos), 2.0)), 0.0) * F2_COLOR_MULT.r;
    color.g = max(1.0 / (1.0 + F2_SHARPNESS * pow(length(uvd + F2_OFFSET.g * pos), 2.0)), 0.0) * F2_COLOR_MULT.g;
    color.b = max(1.0 / (1.0 + F2_SHARPNESS * pow(length(uvd + F2_OFFSET.b * pos), 2.0)), 0.0) * F2_COLOR_MULT.b;
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
            element = spotsPattern(uvx_5, currentPos, F4_OFFSET, F4_POWER, F4_COLOR_MULT);
        }
        else if (pattern == 2)
        {
            element = spotsPattern(uvx_4, currentPos, F5_OFFSET, F5_POWER, F5_COLOR_MULT);
        }
        else
        {
            element = spotsPattern(uvx_5, currentPos, F6_OFFSET, F6_POWER, F6_COLOR_MULT);
        }

        // 거리 감쇄 및 합성
        totalColor += element * weight * pow(falloff, GHOST_FALLOFF_POW) * intensityScale;
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
    float3 distortion = DISTORTION;
    
    float3 finalColor = float3(0, 0, 0);

    finalColor += coreGlow(uv);
    finalColor += processLensFeatures(uv, center, ghostVec, direction, distortion);

    return float4(finalColor * finalAlpha, 1.0f);
} // main