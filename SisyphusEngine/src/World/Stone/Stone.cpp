// World/Stone/Stone.cpp
#include "Stone.h"
#include "Position.h"
#include "Model/MeshModel.h"
#include "Shader/Shader.h"
#include "EngineHelper.h" 

/* default */
/////////////////////////////////////////////////////////////////////

Stone::Stone()
	: ActorObject()
{
} // Stone

Stone::~Stone()
{
    Shutdown();
} // ~Stone

bool Stone::Init(MeshModel* model, const std::string& name)
{
    if (ActorObject::Init(model, name) == false)
        return false;
    

    actor_Position->SetPosition(0.0f, 0.0f, 0.0f);

    return true;
} // Init


void Stone::Shutdown()
{
    ActorObject::Shutdown();
} // Shutdown


void Stone::Frame(float frameTime)
{
    ActorObject::Frame(frameTime);
} // Frame


void Stone::Render(const ActorRenderParams& params)
{
    ActorObject::Render(params);
    return;
} // Render