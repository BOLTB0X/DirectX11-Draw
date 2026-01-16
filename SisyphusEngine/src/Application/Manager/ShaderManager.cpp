// Application/ShaderManager/ShaderManager.cpp
#include "ShaderManager.h"
// Framework
#include "Shader/Shader.h"
// Graphics
#include "Shader/ActorsShader.h"
#include "Shader/TerrainShader.h"
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
    auto terrainShader = std::make_unique<TerrainShader>();
    if (terrainShader->Init(
        device,
        hwnd,
        EngineSettings::SHADER_PATH + EngineSettings::TERRAIN_VS,
        EngineSettings::SHADER_PATH + EngineSettings::TERRAIN_PS)
        == false) return false;
    m_shaders["Terrain"] = std::move(terrainShader);


    auto actorsShader = std::make_unique<ActorsShader>();
    if (actorsShader->Init(
        device,
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