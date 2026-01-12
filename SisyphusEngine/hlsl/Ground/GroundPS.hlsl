// hlsl/Ground/GroundPS.hlsl
Texture2D shaderTextures[5] : register(t0); // 0:Albedo, 1:Normal, 2:Metallic, 3:Roughness, 4:AO
SamplerState SampleType : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPos : TEXCOORD1;
};

float4 PS_Main(PS_INPUT input) : SV_TARGET
{
    float4 albedo = shaderTextures[0].Sample(SampleType, input.tex);
    float3 normalMap = shaderTextures[1].Sample(SampleType, input.tex).rgb;
    float roughness = shaderTextures[3].Sample(SampleType, input.tex).r;
    float ao = shaderTextures[4].Sample(SampleType, input.tex).r;
    
    float3 lightDir = normalize(float3(0.5f, -1.0f, 0.5f));
    float diffuseIntensity = saturate(dot(input.normal, -lightDir));
    float3 finalColor = albedo.rgb * (diffuseIntensity + 0.2f) * ao;

    return float4(finalColor, 1.0f);
} // PS_Main