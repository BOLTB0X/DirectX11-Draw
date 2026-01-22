#define MAX_STEPS 100
#define MARCH_SIZE 0.08
#include "Common.hlsli"

Texture2D uNoise : register(t0); // 노이즈 텍스처
SamplerState uSampler : register(s0); // 샘플러 상태

cbuffer GlobalBuffer : register(b1)
{
    // 1
    float uTime;
    float3 padding;

    // 2
    float3 uCameraPos;
    float padding1;

    // 3
    float2 uResolution;
    float uNoiseRes;
    float padding2;
}; // GlobalBuffer


cbuffer LightBuffer : register(b2)
{
    float3 uLightPos;
    float uIntensity;
    float4 uLightColor;
}; // LightBuffer

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 worldPos : POSITION;
}; // PixelInput


// FBM (Fractal BrownianMotion)
// 여러 옥타브의 노이즈를 중첩
float fbm(float3 p, float time)
{
    // 시간에 따른 흐름 추가
    float3 q = p + time * 0.5f * float3(1.0f, -0.2f, -1.0f);
    
    float f = 0.0f;
    float scale = 0.5f;
    float factor = 2.02f;

    for (int i = 0; i < 6; i++)
    {
        f += scale * GetNoise(q, uNoise, uSampler, 256.0f);
        q *= factor;
        factor += 0.21f;
        scale *= 0.5f;
    }

    return f;
} // fbm


// 밀도 정의
float scene(float3 p)
{
    float distance = sdSphere(p, 2.5f);
    float f = fbm(p, uTime); // 노이즈 추가
    
    return -distance + f;
} // scene


// Shadow Marching
// 현재 지점에서 태양빛이 얼마나 도달하는지 계산
//float GetLightEnergy(float3 p)
//{
//    float3 lightDir = normalize(uLightPos - p);
//    float totalDensity = 0.0f;
//    float shadowStepSize = 0.15f;
    
//    for (int i = 0; i < 6; i++) // // 태양 방향으로 6단계 탐사
//    {
//        float3 shadowPos = p + lightDir * shadowStepSize * (float) i;
//        totalDensity += scene(shadowPos);
//    }
    
//    // uIntensity를 사용하여 빛의 투과율 결정
//    return exp(-totalDensity * uIntensity);
//} // GetLightEnergy


//float4 raymarch(float3 rayOrigin, float3 rayDirection)
//{
//    float4 res = float4(0, 0, 0, 0);
//    float transmittance = 1.0f;
//    float depth = 0.0f;

//    for (int i = 0; i < MAX_STEPS; i++)
//    {
//        float3 p = rayOrigin + depth * rayDirection;
//        float density = scene(p);

//        if (density > 0.0f)
//        {
//            // 해당 지점의 태양빛 에너지 계산 (Shadow)
//            float lightEnergy = GetLightEnergy(p);
            
//            // 색상 결정 (밀도와 빛 에너지를 곱함)
//            float3 ambient = float3(0.2, 0.25, 0.3); // 구름의 어두운 부분 기본 색
//            float3 cloudColor = lerp(ambient, uLightColor.rgb, lightEnergy);
            
//            float4 color = float4(cloudColor, density);
            
//            // 알파 블렌딩 (Front-to-Back)
//            color.rgb *= color.a;
//            res += color * transmittance;
//            transmittance *= (1.0f - color.a);
            
//            if (transmittance < 0.01f)
//                break;
//        }

//        depth += MARCH_SIZE;
//    }

//    return float4(res.rgb, 1.0f - transmittance);
//} // raymarch

float4 raymarch(float3 rayOrigin, float3 rayDirection)
{
    float4 res = float4(0, 0, 0, 0);
    float depth = 0.0f;
    float3 p = rayOrigin + depth * rayDirection;

    for (int i = 0; i < MAX_STEPS; i++)
    {
        float density = scene(p);

        // 밀도가 0보다 클 경우에만 밀도를 draw
        if (density > 0.0f)
        {

            float3 baseColor = lerp(float3(1, 1, 1), float3(0, 0, 0), density);
            float4 color = float4(baseColor, density);

            color.rgb *= color.a;
            res += color * (1.0f - res.a);
        }

        depth += MARCH_SIZE;

        p = rayOrigin + depth * rayDirection;
    } // for (int i = 0; i < MAX_STEPS; i++)

    return res;
} // raymarch


float4 main(PixelInput input) : SV_TARGET
{
    // 화면 좌표 정규화
    //float2 uv = input.position.xy / uResolution.xy;
    //uv -= 0.5f;
    //uv.y *= -1.0f; // DX 좌표계 보정
    //uv.x *= (uResolution.x / uResolution.y);

    float3 ro = uCameraPos;
    float3 rd = normalize(input.worldPos - uCameraPos);
    
    // Ray Origin 및 Ray Direction
    //float3 ro = float3(uCameraPos.x, uCameraPos.y, -uCameraPos.z);
    //float3 rd = normalize(float3(uv, -1.0f));

    float4 res = raymarch(ro, rd);
    return float4(res.rgb, 1.0f);
} // main