struct VSInput
{
    float3 pos : POSITION;
    float3 nor : NORMAL;
};

struct PSInput
{
    float4 pos : SV_POSITION;
};

PSInput VSMain(VSInput input)
{
    PSInput output;
    output.pos = float4(input.pos, 1.0f);
    return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return float4(0.7f, 0.7f, 0.7f, 1.0f);
}