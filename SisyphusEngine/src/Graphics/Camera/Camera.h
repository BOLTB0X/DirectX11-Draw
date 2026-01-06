// Graphics/Camera/Camera.h
#pragma once
#include "Base/Position.h"

class Camera : public Position {
public:
    Camera();
	Camera(const Camera&) = delete;
    ~Camera();

	void InitProjection(int, int, float, float);
    void Render();

public:
    DirectX::XMMATRIX GetViewMatrix() const { return m_viewMatrix; }
    DirectX::XMMATRIX GetProjectionMatrix() const { return m_projectionMatrix; }
    DirectX::XMFLOAT3 GetPosition() const { return Position::GetPosition(); }

private:
    DirectX::XMMATRIX m_viewMatrix;
    DirectX::XMMATRIX m_projectionMatrix;

    float m_fieldOfView;
    float m_screenNear;
    float m_screenDepth;
}; // Camera