// 바인딩된 리소스만 사용
//Texture2D CloudMap : register(t0); // FBX 기본 디퓨즈 텍스처
Texture2D NormalMap : register(t1);
Texture2D AlphaMap : register(t4); // material.alpha 바인딩 텍스처
SamplerState sampleType : register(s0);

// 엔진 공용 MaterialBuffer (b2)
cbuffer MaterialBuffer : register(b2)
{
    int type;
    float gTime;
    float2 padding;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 worldPos : TEXCOORD1;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 scrollUV = input.texCoord + float2(gTime * 0.005f, gTime * 0.002f);
    
    // 2. 텍스처 샘플링 (Normal과 Alpha만 사용)
    float3 normalSample = NormalMap.Sample(sampleType, scrollUV).rgb;
    float4 alphaSample = AlphaMap.Sample(sampleType, scrollUV);

    // 3. 탄젠트 공간 노말 계산 (입체감 부여)
    // 노말 맵 데이터를 [-1, 1] 범위로 변환
    float3 bumpedNormal = normalSample * 2.0f - 1.0f;
    
    // TBN 행렬을 이용해 월드/로컬 노말 생성
    float3 N = normalize(input.normal);
    float3 T = normalize(input.tangent);
    float3 B = normalize(input.binormal);
    float3x3 TBN = float3x3(T, B, N);
    float3 worldNormal = normalize(mul(bumpedNormal, TBN));

    // 4. 소프트 라이팅 (Wrap Lighting)
    // 흰색 구름이므로 노말에 따른 음영이 매우 중요합니다.
    float3 lightDir = normalize(float3(0.5f, 1.0f, 0.5f)); // 위에서 비추는 빛
    float wrap = 0.5f;
    float dotProduct = dot(worldNormal, lightDir);
    float lightIntensity = saturate((dotProduct + wrap) / (1.0f + wrap));

    // 5. 색상 설정 (알베도 대신 흰색 사용)
    float3 cloudBaseColor = float3(1.0f, 1.0f, 1.0f); // 순백색 구름
    
    // 음영을 적용하되, 구름의 밝은 느낌을 위해 환경광(Ambient)을 높게 잡음
    float3 finalColor = cloudBaseColor * (lightIntensity + 0.3f);

    // 6. 알파 결정 (AlphaMap의 R채널 활용)
    // pow를 사용하여 구름 가장자리를 더 부드럽게 깎아냅니다.
    float finalAlpha = pow(alphaSample.r, 1.5f);

    return float4(finalColor, finalAlpha);
}