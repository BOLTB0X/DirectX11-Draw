#pragma once
#include<directxmath.h>


struct GlobalBuffer {
    // Row 1
    float iTime;
    float iFrame;
    DirectX::XMFLOAT2 iResolution;

    // Row 2
    DirectX::XMFLOAT3 iCameraPos;
    float padding1;
    //float iNoiseRes;

    //// Row 3: iCloudType(4) + Padding(12) = 16 bytes
    //float iCloudType;
    //DirectX::XMFLOAT3 padding;
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
    DirectX::XMFLOAT3 baseColor = { 1.0f, 1.0f, 1.0f };
    float iCloudType;
    // Row 2
    DirectX::XMFLOAT3 ambient = { 0.2f, 0.15f, 0.3f };
    float maxSteps = 100.0f;
    // Row 3
    DirectX::XMFLOAT3 shadowColor = { 0.4f, 0.4f, 0.5f };
    float marchSize = 0.08f;
    // Row 4
    float radius;
    float height;
	float thickness;
    float iNoiseRes;
    // Row 5
    float densityScale;
    DirectX::XMFLOAT3 padding;
}; // CloudBuffer