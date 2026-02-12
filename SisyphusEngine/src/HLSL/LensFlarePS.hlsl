// https://john-chapman-graphics.blogspot.com/2013/02/pseudo-lens-flare.html
// 존 챔피언 코드 기반으로 수정
#include "Common.hlsli"

#define HALF 0.5f
#define DISTORTION float3(-0.005, 0.0, 0.005)
#define STAR_SCALE 0.8f
#define PATTERN1_SCALE 0.35f
#define PATTERN2_SCALE 0.3f
#define PATTERN3_SCALE 0.2f

Texture2D iSceneTex : register(t0);
Texture2D iGhost : register(t1);
Texture2D iGlow : register(t2);
Texture2D iHalo1 : register(t3);
Texture2D iHalo2 : register(t4);
Texture2D iHalo3 : register(t5);
Texture2D iStar : register(t6);
Texture2D iDepthTex : register(t7);
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


// 왜곡 샘플링 함수
float3 sampleDistorted(Texture2D tex, float2 uv, float2 direction, float3 distortion)
{
    return float3(
        tex.Sample(iSampler, uv + direction * distortion.r).r,
        tex.Sample(iSampler, uv + direction * distortion.g).g,
        tex.Sample(iSampler, uv + direction * distortion.b).b
    );
} // SampleDistorted


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
    return dot(color, float3(0.3, 0.59, 0.11));
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
    // 블러 및 임계값이 적용된 평균 휘도
    float avgLuma = calculateSunVisibility();

    // 0.1~0.9 구간에서 부드럽게 가시성 결정
    float visibility = smoothstep(0.1f, 0.9f, avgLuma);

    // 화면 외곽 페이드 아웃
    float d = distance(iSunUV, center);
    float edgeFade = 1.0f - smoothstep(0.45f, 0.75f, d);

    return visibility * edgeFade * iGhostAlpha;
} // checkVisibility


// 광원 정중앙에 고정된 빛 갈라짐 효과
float3 applyStar(float2 uv)
{
    float2 starOffset = iSunUV - uv;
    starOffset.x /= iAspect;
    float starSize = iGlowSize * STAR_SCALE;
    float2 starUV = starOffset / starSize + HALF;

    if (all(starUV >= 0.0f && starUV <= 1.0f))
    {
        return iStar.Sample(iSampler, starUV).rgb * STAR_SCALE;
    }
    return float3(0, 0, 0);
} // ApplyStar


float3 processLensFeatures(float2 uv, float2 center, float2 ghostVec, float2 direction, float3 distortion)
{
    float3 ghostColor = float3(0, 0, 0);

    [unroll(10)]
    for (int i = 0; i < iGhostCount; i++)
    {
        float2 ghostPos = frac(iSunUV + ghostVec * (float) i);
        
        // 감쇄 계수
        float falloff = 1.0f - ((float) i / (float) iGhostCount);
        float weight = 1.0f - smoothstep(0.0f, 0.85f, distance(ghostPos, center));
        
        float2 offset = ghostPos - uv;
        //offset.x /= iAspect;

        float3 texElement = float3(0, 0, 0);
        float currentSize = iGlowSize * max(0.4f, falloff);

        if (i == 0) // 태양 바로 옆의 큰 글로우
        {
            float glowSize = iGlowSize * 3.0f;
            float2 glowUV = offset / glowSize + HALF;

            if (all(glowUV >= 0.0f && glowUV <= 1.0f))
            {
                float3 baseGlow = iGlow.Sample(iSampler, glowUV).rgb * HALF;
        
                // 할로들을 왜곡하여 중첩
                float3 h1 = sampleDistorted(iHalo1, glowUV, direction, distortion * 0.5f);
                float3 h2 = sampleDistorted(iHalo2, glowUV, direction, distortion * 0.8f);
                float3 h3 = sampleDistorted(iHalo3, glowUV, direction, distortion * 1.2f);
        
                texElement = baseGlow + (h1 + h2 + h3) * 0.2f;
        
                // 드리프트 행렬 적용 
                glowUV = mul(float3(glowUV, 1.0f), (float3x3)iLensMatrix).xy;
            }
        }
        else
        {
            int pattern = i % 4;
            
            if (pattern == 1 || pattern == 2 || pattern == 3)
            {
                float haloScale = (pattern == 1) ? PATTERN1_SCALE : (pattern == 2 ? PATTERN2_SCALE : PATTERN3_SCALE);
                currentSize *= haloScale;
            }
            else // 일반 육각 고스트 등
            {
                currentSize *= 0.25f;
            }
            
            float2 localUV = offset / currentSize + HALF;
            localUV = mul(float3(localUV, 1.0f), (float3x3) iLensMatrix).xy;
                
            if (pattern == 1)
                texElement = sampleDistorted(iHalo1, localUV, direction, distortion);
            else if (pattern == 2)
                texElement = sampleDistorted(iHalo2, localUV, direction, distortion);
            else if (pattern == 3)
                texElement = sampleDistorted(iHalo3, localUV, direction, distortion);
            else
                texElement = iGhost.Sample(iSampler, localUV).rgb;
                
            texElement *= pow(falloff, 2.5f);
            
        }
        ghostColor += texElement * weight;
    }
    
    return ghostColor;
} // applyGhosts


float4 main(PixelInput input) : SV_TARGET
{
    float2 uv = input.tex;
    float2 center = float2(HALF, HALF);
    
    //return testDepth();
    
    float finalAlpha = checkVisibility(center);

    if (finalAlpha <= 0.0001f)
        return float4(0, 0, 0, 0);

    float2 ghostVec = (center - iSunUV) * iGhostSpacing;
    float2 direction = normalize(ghostVec);
    float3 distortion = DISTORTION;
    
    float3 finalColor = float3(0, 0, 0);

    finalColor += applyStar(uv);
    finalColor += processLensFeatures(uv, center, ghostVec, direction, distortion);

    return float4(finalColor * finalAlpha, 1.0f);
} // main