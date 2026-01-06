// Graphics/Shader/StoneShader.h
#pragma once
#include "Shader.h"

class StoneShader : public Shader
{
public:
    StoneShader();
	StoneShader(const StoneShader&) = delete;
    virtual ~StoneShader() = default;

    // 초기화: 셰이더 컴파일 및 버퍼/샘플러 생성
    bool Init(ID3D11Device* device, HWND hwnd, const std::wstring& vsPath, const std::wstring& psPath) override;

    // 렌더링 준비
    void Bind(ID3D11DeviceContext* context) override;
}; // StoneShader