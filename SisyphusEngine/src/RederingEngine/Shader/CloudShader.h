#pragma once
#include "ShaderBuffers.h"
// Framework
#include "Shader.h"

class CloudShader : public Shader {
public:
    CloudShader();
    CloudShader(const CloudShader& other) = delete;
    virtual ~CloudShader() = default;

    virtual bool Init(ID3D11Device*, HWND,
        const std::wstring&, const std::wstring&) override;
    virtual void SetShaders(ID3D11DeviceContext*) override;

public:
    void SetConstantBuffers(ID3D11DeviceContext*, ID3D11Buffer*);
    bool UpdateCloudBuffer(ID3D11DeviceContext* context, const CloudBuffer& data);

private:
	bool InitBuffers(ID3D11Device*);

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_cloudBuffer;
}; // CloudShader