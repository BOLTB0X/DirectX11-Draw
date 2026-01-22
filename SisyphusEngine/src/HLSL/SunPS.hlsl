// Constant Buffers

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
}; // PixelInput


float4 main(PixelInput input) : SV_TARGET
{
    float2 uv = input.tex * 2.0f - 1.0f; // -1 ~ 1
    float dist = length(uv);

    // 태양 핵
    float core = smoothstep(0.4f, 0.1f, dist);

    // 광선 효과 로직
    // 각도에 따른 사인파를 이용해 방사형 광선 생성
    float angle = atan2(uv.y, uv.x);
    float rays = sin(angle * 12.0f + uTime * 2.0f) * 0.5f + 0.5f;
    float rayGlow = pow(rays, 3.0f) * exp(-dist * 2.0f) * 0.8f;

    // 전체적인 글로우
    float glow = exp(-dist * 3.5f) * 1.5f;

    float3 finalColor = uLightColor.rgb * uIntensity * (core + glow + rayGlow);
    float alpha = saturate(core + glow * 0.5f);

    return float4(finalColor, alpha);
} // main