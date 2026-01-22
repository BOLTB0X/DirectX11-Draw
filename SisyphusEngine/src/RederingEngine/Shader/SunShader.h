#pragma once
#include "Shader.h"
//#include "Model/Light.h"

class Light;

class SunShader : public Shader {
public:
    SunShader();
    SunShader(const SunShader& other) = delete;
    virtual ~SunShader() override = default;

    virtual bool Init(ID3D11Device* device, HWND hwnd,
        const std::wstring& vsPath, const std::wstring& psPath) override;

public:
    void SetShaders(ID3D11DeviceContext* context);
    void SetConstantBuffers(ID3D11DeviceContext* context);

    ID3D11Buffer* GetLightBuffer() { return m_lightBuffer.Get(); }

    bool UpdateLightBuffer(ID3D11DeviceContext* context, Light* light);

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_lightBuffer;
}; // SunShader