// Application/TextureLoader.cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "TextureLoader.h"
#include "EngineHelper.h"

bool TextureLoader::CreateTextureFromFile(
    ID3D11Device* device,
    ID3D11DeviceContext* context,
    const std::string& filename,
    ID3D11ShaderResourceView** outSRV)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
 
    unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    //if (EngineHelper::SuccessCheck(
    //    imageData, "텍스처 로드: stbi_load 에러")
    //    == false) return false;
    //else
    //{
    //    char msg[256];
    //    sprintf_s(msg, "텍스처 생성 시도: %s (%dx%d, %d channels)\n", filename.c_str(), width, height, channels);
    //    OutputDebugStringA(msg);
    //}

    D3D11_TEXTURE2D_DESC td = {};
    td.Width = width;
    td.Height = height;
    td.MipLevels = 0;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.SampleDesc.Count = 1;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    td.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    HRESULT hr = device->CreateTexture2D(&td, nullptr, texture.GetAddressOf());
    if (EngineHelper::SuccessCheck(hr, "텍스처로드: CreateTexture2D 실패")
        == false)
    {
        stbi_image_free(imageData);
        return false;
    }

    // 초기 데이터 업로드
    unsigned int rowPitch = (width * 4) * sizeof(unsigned char);
    context->UpdateSubresource(texture.Get(), 0, NULL, imageData, rowPitch, 0);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
    srvd.Format = td.Format;
    srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvd.Texture2D.MostDetailedMip = 0;
    srvd.Texture2D.MipLevels = -1;

    hr = device->CreateShaderResourceView(texture.Get(), &srvd, outSRV);

    if (EngineHelper::SuccessCheck(
        hr, "텍스처 로드: CreateShaderResourceView 실패")
        == false) return false;

    context->GenerateMips(*outSRV);
    stbi_image_free(imageData);
    return true;
} // CreateTextureFromFile