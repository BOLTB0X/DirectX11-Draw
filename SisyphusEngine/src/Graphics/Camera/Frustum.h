// Graphics/Camera/Frustum.h
#pragma once
#include <DirectXMath.h>

class Frustum {
public:
    Frustum();
	Frustum(const Frustum&) = delete;
    ~Frustum();

    void Init(float);
    void ConstructFrustum(DirectX::XMMATRIX, DirectX::XMMATRIX);

public:
    // 충돌 체크 함수들 (지형 셀이나 돌의 가시성 판단용)
    bool CheckPoint(float, float, float);
    bool CheckCube(float, float, float, float);
    bool CheckSphere(float, float, float, float);
    bool CheckBoundingBox(float, float, float, float, float, float);
    bool CheckBoundingBoxMinMax(float, float, float, float, float, float);

private:
    float m_screenDepth;
    float m_planes[6][4];
}; // Frustum