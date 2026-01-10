// Graphics/MeshModel/VertexBuffer.h
#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include "Model/Mesh/VertexTypes.h"

class VertexBuffer {
public:
    VertexBuffer();
	VertexBuffer(const VertexBuffer&) = delete;
    ~VertexBuffer();

    bool Init(ID3D11Device*, const std::vector<ModelVertex>&);
    void Bind(ID3D11DeviceContext*);

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
    UINT m_stride;
}; // VertexBuffer
