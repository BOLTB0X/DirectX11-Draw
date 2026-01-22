#include "Light.h"
// Framework
#include "Position.h"


Light::Light()
    : m_color(1.0f, 1.0f, 1.0f, 1.0f),
    m_intensity(1.0f)
{
    m_Position = std::make_unique<Position>();
}

Light::~Light() {}

void Light::Init(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 color, float intensity)
{
    SetPosition(pos);

    SetColor(color.x, color.y, color.z, color.w);
    SetIntensity(intensity);

    //m_Position->SetPosition(0.0f, 5.0f, 10.0f); // 기본 태양 위치
    //m_Position->SetScale(1.0f);

    //m_color = { 1.0f, 0.9f, 0.7f, 1.0f }; // 따뜻한 햇살 색상
    //m_intensity = 2.0f;                  // 강한 광선 효과를 위해 1.0 이상 권장

    return;
} // Init


void Light::SetPosition(DirectX::XMFLOAT3 position)
{
    m_Position->SetPosition(position.x, position.y, position.z);
} // SetPosition


void Light::SetColor(float r, float g, float b, float a)
{
    m_color = DirectX::XMFLOAT4(r, g, b, a);
} // SetColor


void Light::SetIntensity(float intensity)
{
    m_intensity = intensity;
} // SetColor


DirectX::XMFLOAT3 Light::GetPosition()
{
    return m_Position->GetPosition();
} // GetPosition


Position* Light::GetPositionPtr()
{
    return m_Position.get();
} // GetPositionPtr