#pragma once
#include <iostream>
#include <string>


namespace ConstantHelper {
    // 화면 설정
    const bool FULL_SCREEN = false;
    const bool VSYNC_ENABLED = true;

    inline int SCREEN_WIDTH = 800;
    inline int SCREEN_HEIGHT = 600;

    // 클리핑 평면
    const float SCREEN_DEPTH = 1000.0f;
    const float SCREEN_NEAR = 0.1f;
}


namespace ConstantHelper { // 모델 및 텍스처
    const std::string ASSET_PATH = "assets/";
    const std::string NOISE_PATH = ASSET_PATH + "Cloud/noise.png";
    const std::string BLUE_NOISE_PATH = ASSET_PATH + "Cloud/blue-noise.png";

    const std::string GHOST = ASSET_PATH + "Lensflare/ghost.jpg";
    const std::string GLOW = ASSET_PATH + "Lensflare/glow.jpg";
	const std::string HALO1 = ASSET_PATH + "Lensflare/halo1.jpg";
	const std::string HALO2 = ASSET_PATH + "Lensflare/halo2.jpg";
    const std::string HALO3 = ASSET_PATH + "Lensflare/halo3.jpg";
    const std::string STAR = ASSET_PATH + "Lensflare/star.jpg";
}


namespace ConstantHelper { // HLSL
    const std::wstring DEFAULT_VS = L"HLSL/DefaultVS.hlsl";
    const std::wstring QUAD_VS = L"HLSL/QuadVS.hlsl";
    const std::wstring SKY_VS = L"HLSL/SkyVS.hlsl";

    const std::wstring CLOUD_PS = L"HLSL/CloudPS.hlsl";
    const std::wstring BICUBIC_PS = L"HLSL/BicubicPS.hlsl";
    const std::wstring SKY_PS = L"HLSL/SkyPS.hlsl";
	const std::wstring LENSFLARE_PS = L"HLSL/LensFlarePS.hlsl";
};


namespace ConstantHelper { // 셰이더 키
    const DirectX::XMFLOAT3 LightDirection = { -0.3f, -0.6f, 0.7f };
    const DirectX::XMFLOAT3 LightPosition = { -1000.0f, 200.0f, 1500.0f };
    //const DirectX::XMFLOAT3 LightPosition = { -10.0f, 2.0f, 15.0f };
    const DirectX::XMFLOAT4 LightColor = { 1.0f, 0.6f, 0.3f, 1.0f };
    const float LightIntensity = 1.5f;
}


namespace ConstantHelper { // 렌더링 관련

    using namespace DirectX;

    enum class CloudType : uint32_t {
        Default = 0,
        Plane = 1,
        Random = 2
    }; // CloudType


    struct CameraSetup {
        XMFLOAT3 position;
        XMFLOAT3 rotation; // Pitch, Yaw, Roll
    }; // CameraSetup

    inline CloudType cloudType = CloudType::Default;

    // 타입에 따른 카메라 초기 설정 반환
    inline CameraSetup GetCloudCameraSetup()
    {
        CameraSetup setup;

        switch (cloudType) {
        case CloudType::Plane:
            setup.position = { 0.0f, 3.169f, -6.0f };
            setup.rotation = { -0.25f, 322.25f, 3.076f };
            break;
        default:
            setup.position = { 0.0f, 0.0f, -6.0f }; 
            setup.rotation = { 0.0f, 0.0f, 0.0f };
            break;
        }
        return setup;
    } // GetCloudCameraSetup
}
