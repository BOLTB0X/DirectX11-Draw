#pragma once
#include "ShaderBuffers.h"
// Framework
#include "Shader.h"


class LensFlareShader : public Shader {
public:
    LensFlareShader();
	LensFlareShader(const LensFlareShader& other) = delete;
    virtual ~LensFlareShader() override = default;

    virtual bool Init(ID3D11Device*, HWND,
        const std::wstring&, const std::wstring&) override;
    virtual void SetShaders(ID3D11DeviceContext*) override;

public:
    bool UpdateLensFlareBuffer(ID3D11DeviceContext*, const LensFlareBuffer&);
    void SetConstantBuffers(ID3D11DeviceContext*, ID3D11Buffer*);

private:
    bool InitBuffers(ID3D11Device*);

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_lensFlareBuffer;
}; // LensFlareShader