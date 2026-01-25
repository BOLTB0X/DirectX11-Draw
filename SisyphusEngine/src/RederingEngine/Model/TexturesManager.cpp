#include "Pch.h"
#include "TexturesManager.h"
#include "Texture.h"
// comonm
#include "DebugHelper.h"
#include "ConstantHelper.h"

TexturesManager::TexturesManager() {}


TexturesManager::~TexturesManager()
{
    Shutdown();
}


bool TexturesManager::Init(ID3D11Device* device, ID3D11DeviceContext* context)
{
    m_Textures = std::unordered_map<std::string, std::shared_ptr<Texture>>();
    if (GetTexture(device, context, ConstantHelper::NOISE_PATH) == nullptr)
    {
        DebugHelper::DebugPrint("TexturesManager::Init 실패: " + ConstantHelper::NOISE_PATH);
        return false;
    }
    return true;
} // Init


void TexturesManager::Shutdown()
{
    m_Textures.clear();
} // Shutdown


std::shared_ptr<Texture> TexturesManager::GetTexture(
    ID3D11Device* device,
    ID3D11DeviceContext* context,
    const std::string& filename)
{
    auto it = m_Textures.find(filename);
    if (it != m_Textures.end())
        return it->second;

    auto newTexture = std::make_shared<Texture>();
    if (newTexture->Init(device, context, filename))
    {
        m_Textures[filename] = newTexture;
        return newTexture;
    }

    return nullptr;
} // GetTexture


void TexturesManager::PSSetShaderResources(ID3D11DeviceContext* context, const std::string& filename, UINT slot)
{
    auto it = m_Textures.find(filename);
    if (it != m_Textures.end() && it->second != nullptr)
        it->second->PSSetShaderResources(context, slot);
    else
        DebugHelper::DebugPrint("TexturesManager::PSSetShaderResources 실패: " + filename + " 이 로드 X");
    
} // PSSetShaderResources