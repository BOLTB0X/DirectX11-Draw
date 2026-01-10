// Graphics/MeshModel/VertexBuffer.cpp
#include "VertexBuffer.h"

/* defualt */
////////////////////////////////////////////////////////////////////

VertexBuffer::VertexBuffer() :m_stride(0) {}

VertexBuffer::~VertexBuffer() {}


bool VertexBuffer::Init(ID3D11Device* device, const std::vector<ModelVertex>& vertices)
{
	if (vertices.empty()) return false;

	D3D11_BUFFER_DESC bufDes = {};

	m_stride = sizeof(ModelVertex);
	bufDes.Usage = D3D11_USAGE_DEFAULT;
	bufDes.ByteWidth = m_stride * (UINT)vertices.size();
	bufDes.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDes.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pSysMem = vertices.data();
	
	HRESULT hr = device->CreateBuffer(&bufDes, &subResourceData, m_buffer.GetAddressOf());
	return SUCCEEDED(hr);
} // Init


void VertexBuffer::Bind(ID3D11DeviceContext* context)
{
	UINT offset = 0;
	ID3D11Buffer* vbs[] = { m_buffer.Get() };
	context->IASetVertexBuffers(0, 1, vbs, &m_stride, &offset);
} // Bind