#pragma once
#include <memory>
#include <directxmath.h>

class Position;


struct LightBuffer {
    DirectX::XMFLOAT3 position;
    float intensity;
    DirectX::XMFLOAT4 color;
}; // LightBuffer


class Light {
public:
    Light();
    Light(const Light& other) = delete;
    ~Light();

    void Init(DirectX::XMFLOAT3, DirectX::XMFLOAT4, float);

public:
    void SetPosition(DirectX::XMFLOAT3);
    void SetColor(float, float, float, float a = 1.0f);
    void SetIntensity(float);


    DirectX::XMFLOAT3 GetPosition();
    Position* GetPositionPtr();
    DirectX::XMFLOAT4 GetColor() const { return m_color; }
    float GetIntensity() const { return m_intensity; }

private:
    std::unique_ptr<Position> m_Position;
    DirectX::XMFLOAT4 m_color;
    float m_intensity;
}; // Light