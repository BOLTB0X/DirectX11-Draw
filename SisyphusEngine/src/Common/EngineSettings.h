// Common/EngineSeetings.h
#pragma once
#include <iostream>
#include <string>

namespace EngineSettings {
    // 화면 설정
    const bool FULL_SCREEN = false;
    const bool VSYNC_ENABLED = true;

    // 기본 해상도
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    // 클리핑 평면
    const float SCREEN_DEPTH = 1000.0f;
    const float SCREEN_NEAR = 0.1f;

    const std::string ASSET_PATH = "assets/";
    const std::wstring SHADER_PATH = L"hlsl/";

    const std::string STONE_PATH = ASSET_PATH + "Stone/Stone.fbx";
    const std::string TERRAIN_PATH = ASSET_PATH + "Terrain/setup.txt";
    const std::string MOUNTAIN_PATH = ASSET_PATH + "Mountain/Mountain.fbx";

    const std::string TERRAINTEX_PATH = ASSET_PATH + "Terrain/textures/";
    const std::string TERRAINNORM_PATH = TERRAINTEX_PATH + "dirt01n.tga";

    const std::wstring COLOR_VS = L"ColorVS.hlsl";
    const std::wstring COLOR_PS = L"ColorPS.hlsl";

    const std::wstring STONE_VS = L"Stone/StoneVS.hlsl";
    const std::wstring STONE_PS = L"Stone/StonePS.hlsl";

    const std::wstring ACTORS_VS = L"Actors/ActorsVS.hlsl";
    const std::wstring ACTORS_PS = L"Actors/ActorsPS.hlsl";

    const std::wstring TERRAIN_VS = L"Terrain/TerrainVS.hlsl";
    const std::wstring TERRAIN_PS = L"Terrain/TerrainPS.hlsl";
}