// Graphics/Model/Texture/Material.h
#pragma once
#include "Texture.h"

#include <string>
#include <memory>


struct Material {
    std::string name;
    std::shared_ptr<Texture> diffuse; // 확산
    std::shared_ptr<Texture> ambient; // 주변광 (그늘/환경)
    std::shared_ptr<Texture> specular; // 반사/투사 (반짝임)
    std::shared_ptr<Texture> albedo;
    std::shared_ptr<Texture> normal; // 노멀 (표면 굴곡)
    float roughness = 1.0f;
    float metalness = 0.0f;
};