// Application/ShaderManager/ShaderManager.cpp
#include "ShaderManager.h"
// Graphics
#include "Shader/Shader.h"
#include "Shader/StoneShader.h"
#include "Shader/ColorShader.h"
#include "Shader/ActorsShader.h"
// Common
#include "EngineSettings.h"

/* default */
///////////////////////////////////////////////////

ShaderManager::ShaderManager() {}


ShaderManager::~ShaderManager()
{ 
    Shutdown();
} // ShaderManager


bool ShaderManager::Init(ID3D11Device* device, HWND hwnd)
{
    auto stoneShader = std::make_unique<StoneShader>();
    if (stoneShader->Init(device,
        hwnd,
        EngineSettings::SHADER_PATH + EngineSettings::STONE_VS,
        EngineSettings::SHADER_PATH + EngineSettings::STONE_PS)
        == false) return false;
    
    m_shaders["Stone"] = std::move(stoneShader);

    //auto colorShader = std::make_unique<ColorShader>();
    //if (colorShader->Init(device,
    //    hwnd,
    //    EngineSettings::SHADER_PATH + EngineSettings::COLOR_VS,
    //    EngineSettings::SHADER_PATH + EngineSettings::COLOR_PS)
    //    == false) return false;

    //m_shaders["Color"] = std::move(colorShader);

    auto actorsShader = std::make_unique<ActorsShader>();
    if (actorsShader->Init(device,
        hwnd,
        EngineSettings::SHADER_PATH + EngineSettings::ACTORS_VS,
        EngineSettings::SHADER_PATH + EngineSettings::ACTORS_PS)
        == false) return false;

    m_shaders["Actors"] = std::move(actorsShader);
    return true;
} // Init

void ShaderManager::Shutdown()
{
    m_shaders.clear();
} // Shutdown