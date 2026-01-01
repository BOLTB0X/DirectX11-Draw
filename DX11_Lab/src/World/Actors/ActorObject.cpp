#include "ActorObject.h"

ActorObject::ActorObject()
{
    m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
    m_worldMatrix = XMMatrixIdentity();
    m_modelResource = nullptr;
} // ActorObject

ActorObject::~ActorObject()
{
    
} // ~ActorObject

void ActorObject::SetPosition(float x, float y, float z)
{
    m_position = XMFLOAT3(x, y, z);
    UpdateWorldMatrix();
} // SetPosition

void ActorObject::SetRotation(float x, float y, float z)
{
    m_rotation = XMFLOAT3(x, y, z);
    UpdateWorldMatrix();
} // SetRotation

void ActorObject::SetScale(float x, float y, float z)
{
    m_scale = XMFLOAT3(x, y, z);
    UpdateWorldMatrix();
} // SetScale

void ActorObject::UpdateWorldMatrix()
{
    XMMATRIX scaling, rotation, translation;

    // 크기 변환
    scaling = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);

    // 회전 변환
    rotation = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);

    // 이동 변환
    translation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

    // 월드 행렬 결합 (S * R * T 순서)
    m_worldMatrix = XMMatrixMultiply(scaling, rotation);
    m_worldMatrix = XMMatrixMultiply(m_worldMatrix, translation);
} // UpdateWorldMatrix