// https://john-chapman-graphics.blogspot.com/2013/02/pseudo-lens-flare.html
// 존 챔피언 코드 기반으로 수정
#include "Common.hlsli"

Texture2D iSceneTex : register(t0);
Texture2D iGhost : register(t1);
Texture2D iGlow : register(t2);
Texture2D iHalo1 : register(t3);
Texture2D iHalo2 : register(t4);
Texture2D iHalo3 : register(t5);
Texture2D iStar : register(t6);
Texture2D iDepthTex : register(t7);
SamplerState iSampler : register(s0);


cbuffer GhostBuffer : register(b3)
{
    int iGhostCount;
    float iGhostSpacing;
    float iGhostThreshold;
    float iGhostAlpha;
    
    float iAspect;
    float iGlowSize;
    float2 iSunUV;
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


float checkVisibility(float2 center)
{
    // 태양 위치의 휘도 계산
    float3 sceneColorAtSun = iSceneTex.Sample(iSampler, iSunUV).rgb;
    float sunBrightness = dot(sceneColorAtSun, float3(0.3, 0.59, 0.11));

    float visibility = smoothstep(0.2f, 0.7f, sunBrightness);

    float d = distance(iSunUV, center);
    float edgeFade = 1.0f - smoothstep(0.45f, 0.75f, d);

    return visibility * edgeFade * iGhostAlpha;
} // CheckVisibility


// 광원 정중앙에 고정된 빛 갈라짐 효과
float3 applyStar(float2 uv)
{
    float2 starOffset = iSunUV - uv;
    starOffset.x /= iAspect;
    float starSize = iGlowSize * 0.8f;
    float2 starUV = starOffset / starSize + 0.5f;

    if (all(starUV >= 0.0f && starUV <= 1.0f))
    {
        return iStar.Sample(iSampler, starUV).rgb * 0.8f;
    }
    return float3(0, 0, 0);
} // ApplyStar


float3 applyGhosts(float2 uv, float2 center, float2 ghostVec, float2 direction, float3 distortion)
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
        offset.x /= iAspect;

        float3 texElement = float3(0, 0, 0);
        float currentSize = iGlowSize * max(0.4f, falloff);

        if (i == 0) // 태양 바로 옆의 큰 글로우
        {
            currentSize = iGlowSize * 3.0f;
            float2 localUV = offset / currentSize + 0.5f;
            if (all(localUV >= 0.0f && localUV <= 1.0f))
                texElement = iGlow.Sample(iSampler, localUV).rgb * 0.4f;
        }
        else
        {
            int pattern = i % 4;
            
            if (pattern == 1 || pattern == 2 || pattern == 3)
            {
                float haloScale = (pattern == 1) ? 0.9f : (pattern == 2 ? 0.7f : 0.5f);
                currentSize *= haloScale;
            }
            else // 일반 육각 고스트 등
            {
                currentSize *= 0.5f;
            }
            
            float2 localUV = offset / currentSize + 0.5f;
            if (all(localUV >= 0.0f && localUV <= 1.0f))
            {
                if (pattern == 1)
                    texElement = sampleDistorted(iHalo1, localUV, direction, distortion);
                else if (pattern == 2)
                    texElement = sampleDistorted(iHalo2, localUV, direction, distortion);
                else if (pattern == 3)
                    texElement = sampleDistorted(iHalo3, localUV, direction, distortion);
                else
                    texElement = iGhost.Sample(iSampler, localUV).rgb;
                
                texElement *= pow(falloff, 1.8f);
            }
        }
        ghostColor += texElement * weight;
    }
    
    return ghostColor;
} // applyGhosts


float4 main(PixelInput input) : SV_TARGET
{
    float2 uv = input.tex;
    float2 center = float2(0.5, 0.5);
    
    //return testDepth();
    
    float finalAlpha = checkVisibility(center);

    if (finalAlpha <= 0.0001f)
        return float4(0, 0, 0, 0);
    

    float2 ghostVec = (center - iSunUV) * iGhostSpacing;
    float2 direction = normalize(ghostVec);
    float3 distortion = float3(-0.015, 0.0, 0.015);
    
    float3 finalColor = float3(0, 0, 0);

    finalColor += applyStar(uv);
    finalColor += applyGhosts(uv, center, ghostVec, direction, distortion);

    return float4(finalColor * finalAlpha, 1.0f);
} // main