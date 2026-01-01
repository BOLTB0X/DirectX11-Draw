#ifndef _ACTOROBJECT_H_
#define _ACTOROBJECT_H_

#include <directxmath.h>
#include "Common/CommonType.h"
#include "Graphics/Resources/Model/Model.h"

using namespace DirectX;

class ActorObject
{
public:
    ActorObject();
    virtual ~ActorObject();

    virtual bool Update(float) = 0;

    // 공통 기능: 위치, 회전, 크기 제어
    void SetPosition(float, float, float);
    void SetRotation(float, float, float);
    void SetScale(float, float, float);

protected:
    // 자식 클래스에서 행렬을 계산할 때 사용
    void UpdateWorldMatrix();

    Model* GetModel() { return m_modelResource; }
    XMFLOAT3 GetPosition() const { return m_position; }
    XMFLOAT3 GetRotation() const { return m_rotation; }
    XMFLOAT3 GetScale() const { return m_scale; }
    XMMATRIX GetWorldMatrix() const { return m_worldMatrix; }

protected:
    // 공통 데이터
    XMFLOAT3 m_position;
    XMFLOAT3 m_rotation;
    XMFLOAT3 m_scale;
    XMMATRIX m_worldMatrix;

    // 공유 리소스 (ResourceManager에서 주소를 받아옴)
    Model* m_modelResource;
};

#endif