#include "Pch.h"
#include "CameraWidget.h"
#include <DirectXMath.h>
// Common
#include "ConstantHelper.h"
// Imgui
#include "imgui.h"


using namespace PropertyHelper;
using namespace DirectX;
using namespace ImGui;


CameraWidget::CameraWidget(
    const std::string& name,
    Property<XMFLOAT3> posProp,
    Property<XMFLOAT3> rotProp,
    Property<float> fovProp)
    : IWidget(name),
    m_posProp(posProp),
    m_rotProp(rotProp),
    m_fovProp(fovProp)
{
} // CameraWidget


void CameraWidget::Frame()
{
    Begin(i_name.c_str());

    XMFLOAT3 pos = m_posProp.Get();
    XMFLOAT3 rot = m_rotProp.Get();
    float fov = m_fovProp.Get();

    Text("Camera Location");

    bool isChanged = false;
    if (DragFloat3("Pos", &pos.x, 0.1f)) isChanged = true;
    if (DragFloat3("Rot", &rot.x, 0.1f)) isChanged = true;
    if (DragFloat("FOV", &fov, 0.5f, 5.0f, 170.0f))
        isChanged = true;

    if (isChanged)
    {
        m_posProp.Set(pos);
        m_rotProp.Set(rot);
        m_fovProp.Set(fov);
    }

    Separator();

    if (Button("Reset Camera"))
    {
        auto setup = ConstantHelper::GetCloudCameraSetup();
        m_posProp.Set(setup.position);
        m_rotProp.Set(setup.rotation);
    }

    End();
} // Frame
