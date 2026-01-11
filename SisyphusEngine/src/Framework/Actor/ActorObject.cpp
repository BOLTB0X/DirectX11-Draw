// Framework/ActorObject.cpp
#include "ActorObject.h"
#include "Position.h"
#include "Model/MeshModel.h"
#include "Shader/Shader.h"
#include "Shader/ActorsShader.h"
#include "EngineHelper.h"

/* default */
/////////////////////////////////////////////////////////////////

ActorObject::ActorObject()
    :actor_name(""),
    actor_Model(nullptr)
{
    actor_Position = std::make_unique<Position>();
} // ActorObject


ActorObject::~ActorObject() {}


bool ActorObject::Init(MeshModel* model, const std::string& name)
{
    if (model == nullptr) return false;

    actor_Model = model;
    actor_name = name;

	actor_Position->SetPosition(0.0f, 0.0f, 0.0f);
	actor_Position->SetRotation(0.0f, 0.0f, 0.0f);
    actor_Position->SetScale(1.0f);

    return true;
} // Init

void ActorObject::Shutdown()
{
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


void ActorObject::Render(const ActorRenderParams& params)
{
    if (EngineHelper::SuccessCheck(actor_Model, "ActorObject::Render -> actor_Model nullptr")
        == false) return;
    if (EngineHelper::SuccessCheck(params.context, "ActorObject::Render -> context nullptr")
        == false) return;

    ActorsShader* targetShader = params.shader;
    if (EngineHelper::SuccessCheck(targetShader, "ActorObject::Render -> targetShader nullptr")
        == false) return;

    targetShader->UpdateMatrixBuffer(
        params.context,
        actor_Position->GetWorldMatrix(),
        params.view,
        params.projection
    );

    targetShader->Bind(params.context);
    actor_Model->Render(params.context, targetShader);
} // Render
