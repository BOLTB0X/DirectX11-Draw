#include "Actors.hlsli"

Texture2D AlbedoMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D MetallicMap : register(t2);
Texture2D RoughnessMap : register(t3);
Texture2D AOMap : register(t4);
SamplerState DefaultSampler : register(s0);

// 머티리얼 타입 정의
#define TYPE_BASE  0
#define TYPE_CLIFF 1
#define TYPE_CLOUD 2
#define TYPE_STONE 3

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 albedo = AlbedoMap.Sample(DefaultSampler, input.tex);
    float ao = AOMap.Sample(DefaultSampler, input.tex).r;
    float roughness = RoughnessMap.Sample(DefaultSampler, input.tex).g;
    
    if (gMaterialType == TYPE_CLOUD) // 구름
    {
        float3 cloudGray = lerp(float3(0.6f, 0.6f, 0.6f), float3(1.0f, 1.0f, 1.0f), roughness);
        
        float3 finalCloudColor = cloudGray * ao;

        float alphaMask = albedo.r;
        if (alphaMask < 0.1f)
            discard;

        return float4(finalCloudColor, alphaMask);
    }

    float3 normalSample = NormalMap.Sample(DefaultSampler, input.tex).rgb;
    float3 localNormal = normalSample * 2.0f - 1.0f;
    
    float3 N = normalize(input.normal);
    float3 T = normalize(input.tangent);
    float3 B = normalize(input.binormal);
    float3x3 TBN = float3x3(T, B, N);
    float3 worldNormal = normalize(mul(localNormal, TBN));

    if (gMaterialType == TYPE_STONE)
    {
        roughness *= 1.2f;
    }

    float3 lightDir = normalize(float3(1.0f, 1.0f, -1.0f));
    float diff = max(dot(worldNormal, lightDir), 0.2f);
    
    float3 finalColor = albedo.rgb * diff * ao;
    
    return float4(finalColor, 1.0f);
} // PS_Main