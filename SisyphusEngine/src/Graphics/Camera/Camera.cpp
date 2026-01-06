// Graphics/Camera/Camera.cpp
#include "Camera.h"

/* default */
////////////////////////////////////////////////////

Camera::Camera(): Position(),
    m_viewMatrix(DirectX::XMMatrixIdentity()),
    m_projectionMatrix(DirectX::XMMatrixIdentity()),
	m_fieldOfView(DirectX::XM_PIDIV4),
	m_screenNear(0.1f),
	m_screenDepth(1000.0f)
{
} // Camera


Camera::~Camera() {} // ~Camera


void Camera::InitProjection(
    int width, int height,
    float screenNear, float screenDepth)
{
    m_screenNear = screenNear;
    m_screenDepth = screenDepth;
    float aspectRatio = (float)width / (float)height;

    m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fieldOfView, aspectRatio, screenNear, screenDepth);
} // InitProjection


void Camera::Render()
{
    DirectX::XMFLOAT3 pos = Position::GetPosition();
    DirectX::XMFLOAT3 rot = Position::GetRotation();

    DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    DirectX::XMVECTOR position = DirectX::XMVectorSet(pos.x, pos.y, pos.z, 0.0f);
    DirectX::XMVECTOR lookAt = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    float pitch = MathHelper::ToRadians(rot.x);
    float yaw = MathHelper::ToRadians(rot.y);
    float roll = MathHelper::ToRadians(rot.z);

    DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

    lookAt = DirectX::XMVector3TransformCoord(lookAt, rotationMatrix);
    up = DirectX::XMVector3TransformCoord(up, rotationMatrix);

    lookAt = DirectX::XMVectorAdd(position, lookAt);

    m_viewMatrix = DirectX::XMMatrixLookAtLH(position, lookAt, up);
    return;
} // Render