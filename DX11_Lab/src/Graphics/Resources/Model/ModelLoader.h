////////////////////////////////////////////////////////////////////////////////
// Filename: ModelLoader.h
////////////////////////////////////////////////////////////////////////////////

/// <summary>
/// 파일을 읽어 MeshData를 채워주는 유틸리티 클래스
/// </summary>

#ifndef _MODELLOADER_H_
#define _MODELLOADER_H_

//////////////
// INCLUDES //
//////////////

#include "Common/CommonType.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ModelLoader {
public:
    static bool LoadFromFile(const std::string& filename, MeshData& outData);

private:
    static void ProcessMesh(aiMesh* mesh, const aiScene* scene, MeshData& outData);
};

#endif