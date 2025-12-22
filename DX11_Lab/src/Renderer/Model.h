#pragma once
#include <vector>
#include <string>
#include "Mesh.h"

class Model
{
public:
    bool Load(ID3D11Device* device, const std::string& path);
    void Draw(ID3D11DeviceContext* context);

private:
    std::vector<Mesh> mMeshes;
};
