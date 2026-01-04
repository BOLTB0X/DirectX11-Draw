// Graphics/Model/VertexBuffer.cpp
#include "VertexBuffer.h"

/* defualt */
////////////////////////////////////////////////////////////////////

VertexBuffer::VertexBuffer() :m_stride(0) {}

VertexBuffer::~VertexBuffer() {}


bool VertexBuffer::Init(ID3D11Device* device, const std::vector<ModelVertex>& vertices) {
	if (vertices.empty()) return false;

	m_stride = sizeof(ModelVertex);
	D3D11_BUFFER_DESC bufDes = {};

	bufDes.Usage = D3D11_USAGE_DEFAULT;
	bufDes.ByteWidth = m_stride * (UINT)vertices.size();
	bufDes.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDes.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices.data();
	
	HRESULT hr = device->CreateBuffer(&bufDes, &sd, m_buffer.GetAddressOf());
	return SUCCEEDED(hr);
} // Init


void VertexBuffer::Bind(ID3D11DeviceContext* context) {
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, m_buffer.GetAddressOf(), &m_stride, &offset);
} // Bind