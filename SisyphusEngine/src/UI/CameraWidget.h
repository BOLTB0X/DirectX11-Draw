#pragma once
#include "IWidget.h"
//
#include <DirectXMath.h>
// Common
#include "PropertyHelper.h"

class Camera;

class CameraWidget : public IWidget {
public:
    CameraWidget(const std::string&,
        PropertyHelper::Property<DirectX::XMFLOAT3>,
        PropertyHelper::Property<DirectX::XMFLOAT3>,
        PropertyHelper::Property<float>);
    CameraWidget(const CameraWidget& other) = delete;

    virtual ~CameraWidget() = default;
    virtual void Frame() override;

private:
private:
    PropertyHelper::Property<DirectX::XMFLOAT3> m_posProp;
    PropertyHelper::Property<DirectX::XMFLOAT3> m_rotProp;
    PropertyHelper::Property<float> m_fovProp;
}; // CameraWidget