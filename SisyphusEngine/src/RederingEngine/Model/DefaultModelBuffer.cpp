#include "Pch.h"
#include "DefaultModelBuffer.h"


DefaultModelBuffer::DefaultModelBuffer()
    : m_vertexCount(4),
    m_indexCount(6)
{}

DefaultModelBuffer::~DefaultModelBuffer() { Shutdown(); }


bool DefaultModelBuffer::Init(ID3D11Device* device, DefaultModelType type)
{
    std::vector<VertexType> vertices;
    std::vector<unsigned long> indices;

    switch (type)
    {
    case  DefaultModelType::Quad:
        CreateQuad(vertices, indices);
        break;
    case DefaultModelType::Cube:
        CreateCube(vertices, indices);
        break;
    case DefaultModelType::Sphere:
        CreateSphere(vertices, indices);
        break;
    }

    m_vertexCount = static_cast<int>(vertices.size());
    m_indexCount = static_cast<int>(indices.size());

    return InitBuffers(device, vertices, indices);
} // Init


void DefaultModelBuffer::Shutdown()
{
    m_indexBuffer.Reset();
    m_vertexBuffer.Reset();
} // Shutdown


void DefaultModelBuffer::Render(ID3D11DeviceContext* context)
{
    unsigned int stride = sizeof(VertexType);
    unsigned int offset = 0;

    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
} // Render


bool DefaultModelBuffer::InitBuffers(
    ID3D11Device* device,
    std::vector<VertexType> vertices,
    std::vector<unsigned long> indices)
{
    // Vertex Buffer 생성
    D3D11_BUFFER_DESC vbd = {};
    vbd.Usage = D3D11_USAGE_DEFAULT;
    vbd.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vinit = { vertices.data() };
    if (FAILED(device->CreateBuffer(&vbd, &vinit, &m_vertexBuffer))) return false;

    // Index Buffer 생성
    D3D11_BUFFER_DESC ibd = {};
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.ByteWidth = sizeof(unsigned long) * m_indexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA iinit = { indices.data() };
    if (FAILED(device->CreateBuffer(&ibd, &iinit, &m_indexBuffer))) return false;

    return true;
} // InitBuffers


void DefaultModelBuffer::CreateQuad(std::vector<VertexType>& vertices, std::vector<unsigned long>& indices, float width, float height)
{
    float w2 = width / 2.0f;
    float h2 = height / 2.0f;

    vertices = {
        { {-w2,  h2, 0.0f}, {0.0f, 0.0f} }, // 좌상
        { { w2,  h2, 0.0f}, {1.0f, 0.0f} }, // 우상
        { {-w2, -h2, 0.0f}, {0.0f, 1.0f} }, // 좌하
        { { w2, -h2, 0.0f}, {1.0f, 1.0f} }  // 우하
    };
    indices = { 0, 1, 2, 2, 1, 3 };
} // CreateQuad


void DefaultModelBuffer::CreateCube(std::vector<VertexType>& vertices, std::vector<unsigned long>& indices, float width, float height, float depth)
{
    float w2 = width / 2.0f;
    float h2 = height / 2.0f;
    float d2 = depth / 2.0f;

    DirectX::XMFLOAT3 p[8] = {
        {-w2,  h2, -d2}, { w2,  h2, -d2}, {-w2, -h2, -d2}, { w2, -h2, -d2}, // 앞면 4개
        {-w2,  h2,  d2}, { w2,  h2,  d2}, {-w2, -h2,  d2}, { w2, -h2,  d2}  // 뒷면 4개
    };

    // 6개 면 정의 (각 면당 정점 4개씩 분리하여 텍스처 좌표 할당)
    vertices = {
        { p[0], {0,0} }, { p[1], {1,0} }, { p[2], {0,1} }, { p[3], {1,1} }, // Front
        { p[5], {0,0} }, { p[4], {1,0} }, { p[7], {0,1} }, { p[6], {1,1} }, // Back
        { p[4], {0,0} }, { p[5], {1,0} }, { p[0], {0,1} }, { p[1], {1,1} }, // Top
        { p[2], {0,0} }, { p[3], {1,0} }, { p[6], {0,1} }, { p[7], {1,1} }, // Bottom
        { p[4], {0,0} }, { p[0], {1,0} }, { p[6], {0,1} }, { p[2], {1,1} }, // Left
        { p[1], {0,0} }, { p[5], {1,0} }, { p[3], {0,1} }, { p[7], {1,1} } // Right
    };

    // 인덱스 생성 (6개 면 * 면당 삼각형 2개)
    for (int i = 0; i < 6; i++)
    {
        int offset = i * 4;
        indices.push_back(offset + 0); indices.push_back(offset + 1); indices.push_back(offset + 2);
        indices.push_back(offset + 2); indices.push_back(offset + 1); indices.push_back(offset + 3);
    }
} // CreateCube


void DefaultModelBuffer::CreateSphere(std::vector<VertexType>& vertices, std::vector<unsigned long>& indices, float radius, int sliceCount, int stackCount)
{
    // 버텍스 생성
    for (int i = 0; i <= stackCount; ++i)
    {
        float phi = DirectX::XM_PI * (float)i / stackCount;
        for (int j = 0; j <= sliceCount; ++j)
        {
            float theta = DirectX::XM_2PI * (float)j / sliceCount;

            VertexType v;
            v.position.x = radius * sinf(phi) * cosf(theta);
            v.position.y = radius * cosf(phi);
            v.position.z = radius * sinf(phi) * sinf(theta);

            v.texture.x = (float)j / sliceCount;
            v.texture.y = (float)i / stackCount;

            vertices.push_back(v);
        } // for (int j = 0; j <= sliceCount; ++j)
    } //  for (int i = 0; i <= stackCount; ++i)

    // 인덱스 생성
    for (int i = 0; i < stackCount; ++i)
    {
        for (int j = 0; j < sliceCount; ++j)
        {
            indices.push_back(i * (sliceCount + 1) + j);
            indices.push_back((i + 1) * (sliceCount + 1) + j);
            indices.push_back(i * (sliceCount + 1) + (j + 1));

            indices.push_back(i * (sliceCount + 1) + (j + 1));
            indices.push_back((i + 1) * (sliceCount + 1) + j);
            indices.push_back((i + 1) * (sliceCount + 1) + (j + 1));
        } // for (int j = 0; j < sliceCount; ++j)
    } // for (int i = 0; i < stackCount; ++i)
} // CreateSphere
