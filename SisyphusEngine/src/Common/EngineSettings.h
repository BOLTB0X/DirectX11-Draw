// Common/EngineSeetings.h
#pragma once

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
}