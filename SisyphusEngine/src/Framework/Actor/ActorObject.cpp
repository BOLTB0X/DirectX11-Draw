// Framework/ActorObject.cpp
#include "ActorObject.h"
#include "Shader/Shader.h"
#include "Position/Position.h"
// Graphics
#include "Shader/ActorsShader.h"
#include "Model/MeshModel.h"
#include "Camera/Camera.h"
// Common
#include "EngineHelper.h"

/* default */
/////////////////////////////////////////////////////////////////

ActorObject::ActorObject()
    :actor_name(""),
    actor_Shader(nullptr),
    actor_Model(nullptr)
{
    actor_Position = std::make_unique<Position>();
} // ActorObject


ActorObject::~ActorObject()
{
    Shutdown();
} // ~ActorObject


bool ActorObject::Init(MeshModel* model, ActorsShader* shader, const std::string& name)
{
    if (model == nullptr || shader == nullptr) return false;

    actor_Model = model;
    actor_name = name;
    actor_Shader = shader;

	actor_Position->SetPosition(0.0f, 0.0f, 0.0f);
	actor_Position->SetRotation(0.0f, 0.0f, 0.0f);
    actor_Position->SetScale(1.0f);

    return true;
} // Init

void ActorObject::Shutdown()
{
    if (actor_Shader) actor_Shader = nullptr;
    if (actor_Model)
    {
        actor_Model = nullptr;
	}


    actor_Position.reset();
    return;
} // Shutdown


void ActorObject::Frame(float frameTime)
{
    if (actor_Position)
    {
        actor_Position->SetFrameTime(frameTime);
    }

    return;
} // Frame


void ActorObject::Render(ID3D11DeviceContext* context, Camera* camera)
{
    if (actor_Model == nullptr || actor_Shader == nullptr) return;
    if (EngineHelper::SuccessCheck(context, "ActorObject::Render -> context nullptr")
        == false) return;
    if (EngineHelper::SuccessCheck(camera, "ActorObject::Render -> camera nullptr")
        == false) return;

    actor_Shader->UpdateMatrixBuffer(
        context,
        actor_Position->GetWorldMatrix(),
        camera->GetViewMatrix(),
        camera->GetProjectionMatrix()
    );

    actor_Shader->Bind(context);
    auto frustum = camera->GetFrustum();
    if (frustum)
    {
        actor_Model->Render(context, actor_Shader, frustum);
    }
    else
    {
        actor_Model->Render(context, actor_Shader);
    }
    return;
} // Render
