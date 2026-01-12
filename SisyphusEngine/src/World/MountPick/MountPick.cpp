#include "MountPick.h"
// Graphics
#include "Model/MeshModel.h"
#include "Model/HeightMap/HeightMap.h"
#include "Shader/ActorsShader.h"
#include "Camera/Camera.h"

/* default */
///////////////////////////////////////////////////////////////////////////////////

MountPick::MountPick()
    : ActorObject(),
    m_heightMap(nullptr) {}

MountPick::~MountPick()
{
    Shutdown();
}

bool MountPick::Init(MeshModel* model, ActorsShader* shader, const std::string& name)
{
    if (ActorObject::Init(model, shader, name)
        == false) return false;

    std::vector<ModelVertex> v;
    std::vector<unsigned int> i;

    model->GetPhysicsData(v, i);

    m_heightMap = std::make_unique<HeightMap>();
    if (m_heightMap->Init(v, i) == false) return false;


    return true;
} // MountPick


void MountPick::Shutdown()
{
    ActorObject::Shutdown();
} // Shutdown


void MountPick::Frame(float frameTime)
{
    ActorObject::Frame(frameTime);
} // Frame


void MountPick::Render(ID3D11DeviceContext* context, Camera* camera)
{
    ActorObject::Render(context, camera);
} // Render

///////////////////////////////////////////////////////////////////////////////////

/* public */
///////////////////////////////////////////////////////////////////////////////////


float MountPick::GetHeightAt(float x, float z) const
{
    if (m_heightMap == nullptr) return 0.0f;
    return m_heightMap->GetHeightAt(x, z);
} // GetHeightAt