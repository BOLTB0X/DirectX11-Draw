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
        sunIntensity = 1.0f;
        rayStrength = 0.4f;
        rayAnimSpeed = 0.15f;
		padding2 = { 0.0f, 0.0f, 0.0f };
	}
}; // SkyBuffer


struct LensFlareBuffer {
    // Row 1
    DirectX::XMFLOAT2 sunPos;
    float threshold;
    float scale;

    // Row 2
    float bias;
    float ghostCount;
    float ghostSpacing;
    float distortion;

    // Row 3
    DirectX::XMFLOAT3 tintColor;
    float padding1;

    // Row 4
    float visibility;
    DirectX::XMFLOAT3 padding2;

    LensFlareBuffer() {
        sunPos = { 0.0f, 0.0f };
        threshold = 0.85f;
        scale = 0.0f; // 기본값은 0
        bias = -0.5f;
        ghostCount = 4.0f;
        ghostSpacing = 0.4f;
        distortion = 2.5f;
        tintColor = { 1.0f, 0.9f, 0.7f };
        padding1 = 0.0f;
        visibility = 0.0f;
        padding2 = { 0.0f, 0.0f, 0.0f };
    } // LensFlareBuffer


    void Init(const DirectX::XMVECTOR& sunWorldPos,
        const DirectX::XMFLOAT3& camPos,
        const DirectX::XMMATRIX& view,
        const DirectX::XMMATRIX& proj)
    {
        using namespace DirectX;

        XMMATRIX invView = XMMatrixInverse(nullptr, view);
        XMVECTOR camForward = invView.r[2]; // 카메라가 바라보는 방향
        XMVECTOR camPosVec = XMLoadFloat3(&camPos);

        // 태양 방향 벡터 (World Space)
        XMVECTOR sunDir = XMVector3Normalize(sunWorldPos);

        // 카메라 전방과 태양 방향의 내적
        // 카메라가 태양을 정면으로 볼 때 1.0, 등지면 -1.0
        float dot = XMVectorGetX(XMVector3Dot(camForward, sunDir));

        //  가시성 결정 (Threshold)
        if (dot < 0.1f)
        {
            this->scale = 0.0f;
            this->visibility = 0.0f;
            return;
        }

        // 정면에 가까울수록 강해지게 설정
        this->visibility = MathHelper::clamp((dot - 0.1f) / 0.9f, 0.0f, 255.0f);
        // 더 극적인 효과를 위해 제곱
        this->visibility *= this->visibility;

        // 투영 계산
        XMVECTOR farSunPos = camPosVec + (sunDir * 10000.0f);
        XMMATRIX world = XMMatrixIdentity();
        XMVECTOR sunScreenPos = XMVector3Project(farSunPos,
            0, 0, (float)ConstantHelper::SCREEN_WIDTH, (float)ConstantHelper::SCREEN_HEIGHT, 0.0f, 1.0f,
            proj, view, world);

        this->sunPos.x = (XMVectorGetX(sunScreenPos) / (float)ConstantHelper::SCREEN_WIDTH) - 0.5f;
        this->sunPos.y = (1.0f - (XMVectorGetY(sunScreenPos) / (float)ConstantHelper::SCREEN_HEIGHT)) - 0.5f;

        this->scale = ConstantHelper::LightIntensity * this->visibility;
    } // Init
}; // LensFlareBuffer