// hlsl/Terrain/TerrainPS.hlsl
Texture2D normalMap : register(t1); // 노말맵 (Material::normal은 t1 슬롯에 바인딩 예상)
SamplerState sampleType : register(s0); // 샘플러 (기본 샘플러는 s0 슬롯에 바인딩 예상)

cbuffer MaterialBuffer : register(b2) // MaterialBuffer는 b2 슬롯에 바인딩 예상
{
    int type; // 현재는 사용하지 않음
    float3 padding;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 worldPos : TEXCOORD1; // 픽셀 셰이더에서 월드 위치 필요시 사용
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 baseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

    float4 normalSample = normalMap.Sample(sampleType, input.texCoord);
    
    float3 bumpNormal = (normalSample.xyz * 2.0f) - 1.0f;
    
    float3x3 TBN = float3x3(input.tangent, input.binormal, input.normal);
    
    float3 worldNormal = normalize(mul(bumpNormal, TBN));


    float3 lightDir = normalize(float3(0.5f, -1.0f, 0.5f));
    float lightIntensity = saturate(dot(worldNormal, -lightDir));
    float3 finalColor = baseColor.xyz * (lightIntensity + 0.2f);

    return float4(finalColor, 1.0f);
} // main