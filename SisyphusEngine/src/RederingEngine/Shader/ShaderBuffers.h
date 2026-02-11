#pragma once
#include<directxmath.h>
// Common
#include "MathHelper.h"
#include "ConstantHelper.h"

struct GlobalBuffer {
    // Row 1
    float iTime;
    float iFrame;
    DirectX::XMFLOAT2 iResolution;

    // Row 2
    DirectX::XMFLOAT3 iCameraPos;
    float padding1;
    //float iNoiseRes;

    DirectX::XMFLOAT4 padding2;
}; // GlobalBuffer


struct MatrixBuffer {
    DirectX::XMMATRIX model;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projection;
}; // MatrixBuffer


struct LightBuffer {
    // Row 1
    DirectX::XMFLOAT3 position;
    float intensity;
    // Row 2
    DirectX::XMFLOAT4 color;
}; // LightBuffer


struct CloudBuffer {
    // Row 1
    DirectX::XMFLOAT3 baseColor;
    float iCloudType;
    // Row 2
    DirectX::XMFLOAT3 ambient;
    float maxSteps;
    // Row 3
    DirectX::XMFLOAT3 shadowColor;
    float marchSize;
    // Row 4
    float radius;
    float height;
	float thickness;
    float iNoiseRes;
    // Row 5
    float densityScale;
    DirectX::XMFLOAT3 padding;

    CloudBuffer(float cloudType)
		: iCloudType(cloudType)
    {
		baseColor = { 1.0f, 1.0f, 1.0f };
		ambient = { 0.2f, 0.15f, 0.3f };
		shadowColor = { 0.4f, 0.4f, 0.5f };
		maxSteps = 100.0f;
		marchSize = 0.08f;
		radius = 2.0f;
		height = 1.0f;
		thickness = 2.0f;
		iNoiseRes = 256.0f;
		densityScale = 0.4f;
		padding = { 0.0f, 0.0f, 0.0f };
    }
}; // CloudBuffer


struct SkyBuffer {
    // Row 1
    DirectX::XMFLOAT3 topColor;
    float padding1;

    // Row 2 하늘
    DirectX::XMFLOAT3 horizonColor;
    float skyExponent;

    // Row 3 태양
    float sunSize;
    float sunBloom; // 태양 주변 광량 확산 속도
    float sunIntensity;
    float rayStrength;

    // Row 4
    float rayAnimSpeed;
    DirectX::XMFLOAT3 padding2;

    SkyBuffer()
     {
        topColor = { 0.05f, 0.1f, 0.3f };
		padding1 = 0.0f;
        horizonColor = { 0.5f, 0.2f, 0.4f };
        skyExponent = 0.3f;
        sunSize = 0.005f;
        sunBloom = 80.0f;
        sunIntensity = 0.8f;
        rayStrength = 0.4f;
        rayAnimSpeed = 0.15f;
		padding2 = { 0.0f, 0.0f, 0.0f };
	}
}; // SkyBuffer


struct ThresholdBuffer {
    DirectX::XMFLOAT4 scale;
    DirectX::XMFLOAT4 bias;

    ThresholdBuffer()
    {
        scale = { 1.0f, 1.0f, 1.0f, 1.0f };
        bias = { -10.0f, -10.0f, -10.0f, 0.0f };
    }
}; // ThresholdBuffer


struct GhostBuffer {
	// Row 1
    int count;
    float spacing;
    float threshold;
    float alpha;
	// Row 2
    float glowSize;
    float aspectRatio;
	DirectX::XMFLOAT2 sunUV;

    GhostBuffer()
    {
        count = 10;
        spacing = 0.5f;
        threshold = 0.3f;
        alpha = 1.0f;

		glowSize = 0.2f;
		aspectRatio = (float)ConstantHelper::SCREEN_WIDTH / (float)ConstantHelper::SCREEN_HEIGHT;
        sunUV = { 0.5f, 0.5f };
    }
}; // GhostBuffer

