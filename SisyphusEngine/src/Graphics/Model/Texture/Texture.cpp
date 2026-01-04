// Graphics/Model/Texture.cpp
#include "Texture.h"
#include "TextureLoader.h"

/* defualt */
///////////////////////////////////////////////////////////////////

Texture::Texture() {};

Texture::~Texture() {};

bool Texture::Init(ID3D11Device* device, ID3D11DeviceContext* context, const std::string& path)
{
	return TextureLoader::CreateTextureFromFile(device, context, path, m_srv.GetAddressOf());
} // Init


void Texture::Bind(ID3D11DeviceContext* context, UINT slot)
{
	context->PSSetShaderResources(slot, 1, m_srv.GetAddressOf());
} // Bind