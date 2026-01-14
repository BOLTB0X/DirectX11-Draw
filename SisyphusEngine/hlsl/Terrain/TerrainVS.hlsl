// hlsl/Terrain/TerrainVS.hlsl
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VS_INPUT
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 worldPos : TEXCOORD1; // 픽셀 셰이더에서 월드 좌표 필요시 사용
};

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output;

    // 위치 변환 (모델 -> 월드 -> 뷰 -> 프로젝션)
	output.position = mul(float4(input.position, 1.0f), worldMatrix);
	output.worldPos = output.position.xyz; // 월드 공간 위치 저장
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

    // 텍스처 좌표는 그대로 전달
	output.texCoord = input.texCoord;

    // 법선, 탄젠트, 바이노말 벡터를 월드 공간으로 변환
    // 노말 맵 계산을 위해 픽셀 셰이더로 전달
	output.normal = normalize(mul(input.normal, (float3x3) worldMatrix));
	output.tangent = normalize(mul(input.tangent, (float3x3) worldMatrix));
	output.binormal = normalize(mul(input.binormal, (float3x3) worldMatrix));

	return output;
} // main