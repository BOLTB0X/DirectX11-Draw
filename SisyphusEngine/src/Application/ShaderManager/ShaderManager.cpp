// Application/ShaderManager/ShaderManager.cpp
#include "ShaderManager.h"
#include "Graphics/Shader/StoneShader.h"
#include "Common/EngineSettings.h"

/* default */
///////////////////////////////////////////////////

ShaderManager::ShaderManager() {}


ShaderManager::~ShaderManager()
{ 
    Shutdown();
} // ShaderManager


bool ShaderManager::Init(ID3D11Device* device, HWND hwnd)
{
    // 1. StoneShader 등록
    auto stoneShader = std::make_unique<StoneShader>();

    std::wstring vsPath = EngineSettings::SHADER_PATH + EngineSettings::STONE_VS;
    std::wstring psPath = EngineSettings::SHADER_PATH + EngineSettings::STONE_PS;
    if (stoneShader->Init(device,
        hwnd,
        vsPath,
        psPath)
        == false) {
        return false;
    }
    m_shaders["Stone"] = std::move(stoneShader);

    return true;
} // Init

void ShaderManager::Shutdown()
{
    m_shaders.clear();
} // Shutdown