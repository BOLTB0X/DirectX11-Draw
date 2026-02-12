cbuffer GlobalBuffer : register(b1)
{
    // Row 1
    float iTime;
    float iFrame;
    float2 iResolution;
    // Row 2
    float3 iCameraPos;
    float padding1;
    // Row 3
    float4 padding2;
};


cbuffer LightBuffer : register(b2)
{
    float3 iLightDirection;
    float iIntensity;
    float4 iLightColor;
}; // LightBuffer


struct PixelInput
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 worldPos : POSITION;
    float3 worldNormal : NORMAL;
}; // PixelInput

