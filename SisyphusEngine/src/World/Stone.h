// World/Stone.h
#pragma once
#include "Framework/ActorObject.h"

class Stone : public ActorObject
{
public:
    Stone();
	Stone(const Stone&) = delete;
    virtual ~Stone();

    virtual bool Init(Model*, const std::string&) override;
    virtual void Frame(float frameTime) override;
    virtual void Render(ID3D11DeviceContext*, Shader*,
        const DirectX::XMMATRIX&, const DirectX::XMMATRIX&) override;

private:
}; // Stone