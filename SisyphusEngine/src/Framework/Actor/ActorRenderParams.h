// Framework/Actor/ActorRenderParams.h
#pragma once
#include <d3d11.h>
#include <directxmath.h>

class Position;
class MeshModel;
class ActorsShader;
class Frustum;


struct ActorRenderParams {
    ID3D11DeviceContext* context;
    ActorsShader* shader;
    Frustum* frustum;

    const DirectX::XMMATRIX& view;
    const DirectX::XMMATRIX& projection;
}; // RenderParams