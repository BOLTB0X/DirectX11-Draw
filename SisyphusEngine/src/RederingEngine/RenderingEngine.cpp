#include "Pch.h"
#include "RenderingEngine.h"
#include "Renderer/Renderer.h"
#include "Model/DefaultModel.h"
#include "Model/Light.h"
#include "Model/TexturesManager.h"
#include "Model/Texture.h"
#include "Shader/ShaderManager.h"
// Common
#include "ConstantHelper.h"
#include "DebugHelper.h"


using namespace PropertyHelper;
using namespace DirectX;


RenderingEngine::RenderingEngine()
    : m_isWireframe(false),
    m_backCullEnable(false),
    m_depthEnable(true),
    m_frameCount(0)
{
    m_Renderer = std::make_unique<Renderer>();
    m_TexturesManager = std::make_unique<TexturesManager>();
    m_ShaderManager = std::make_unique<ShaderManager>();
    m_Cloud = std::make_unique<DefaultModel>();
    m_Quad = std::make_unique<DefaultModel>();
    m_Sky = std::make_unique<DefaultModel>();
    m_Light = std::make_unique<Light>();
} // RenderingEngine


RenderingEngine::~RenderingEngine()
{

} // ~RenderingEngine


bool RenderingEngine::Init(HWND hwnd)
{
    if (m_Renderer->Init(hwnd, true) == false)
        return false;

    if (m_TexturesManager->Init(
        m_Renderer->GetDevice(),
        m_Renderer->GetDeviceContext())
        == false) return false;

    if (m_ShaderManager->Init(
        m_Renderer->GetDevice(),
        hwnd) == false) return false;

    if (m_Sky->Init(
        m_Renderer->GetDevice(), DefaultModelType::Cube) == false)
        return false;

    if (m_Quad->Init(
        m_Renderer->GetDevice(), DefaultModelType::Quad) == false)
        return false;

    if (m_Cloud->Init(
        m_Renderer->GetDevice(), DefaultModelType::Sphere) == false)
        return false;

    m_Light->Init(ConstantHelper::LightPosition, ConstantHelper::LightColor, ConstantHelper::LightIntensity);

    return true;
} // Init


void RenderingEngine::Shutdown()
{
    if (m_ShaderManager)
        m_ShaderManager.reset();

    if (m_TexturesManager)
        m_TexturesManager.reset();

    if (m_Renderer)
    {
        m_Renderer->Shutdown();
        m_Renderer.reset();
    }
    return;
} // Shutdown


void RenderingEngine::Draw(
    float totalTime,
    Property<XMMATRIX> viewProp,
    Property<XMMATRIX> projProp,
    Property<XMFLOAT3> camPosProp)
{
    ID3D11DeviceContext* context = m_Renderer->GetDeviceContext();
    XMFLOAT3 camPos = camPosProp.Get();
    XMMATRIX view = viewProp.Get();
    XMMATRIX proj = projProp.Get();

    m_Renderer->SetLowResolutionRenderTarget();
    m_Renderer->SetAlphaBlending(true);
    m_Renderer->SetDepthBuffer(false);

    // 스카이
    DrawSky(context, totalTime, camPos, view, proj);

    // Back-face Culling
    m_Renderer->SetMode(false, true);

    // 구름
    DrawCloud(context, totalTime, camPos, view, proj);

	// 포스트 프로세싱
    m_Renderer->SetBackBufferRenderTarget();
    m_Renderer->SetAlphaBlending(false);

    ApplyLensFlare(context, view, proj, camPos);
    ApplyBicubicUpscale(context);

    m_Renderer->ClearShaderResources(0);
    m_Renderer->SetDepthBuffer(true);
  
    m_frameCount++;
} // Draw


void RenderingEngine::BeginScene(float r, float g, float b, float a)
{
    m_Renderer->BeginScene(r, g, b, a);
} // BeginScene


void RenderingEngine::EndScene()
{
    m_Renderer->EndScene();
} // EndScene


void RenderingEngine::SetMode(bool isWireframe, bool backCullEnable)
{
    m_Renderer->SetMode(isWireframe, backCullEnable);
} // SetMode


void RenderingEngine::SetDepthBuffer(bool depthEnable)
{
    m_Renderer->SetDepthBuffer(depthEnable);
} // SetDepthBuffer


ID3D11Device* RenderingEngine::GetDevice()
{
    return m_Renderer->GetDevice();
} // GetDevice


ID3D11DeviceContext* RenderingEngine::GetDeviceContext()
{
    return m_Renderer->GetDeviceContext();
} // GetDeviceContext


void RenderingEngine::DrawSky(ID3D11DeviceContext* context,
    float totalTime, XMFLOAT3 camPos, XMMATRIX view, XMMATRIX proj)
{
    using namespace ConstantHelper;

    XMMATRIX skyModel = XMMatrixTranslation(camPos.x, camPos.y, camPos.z);
    m_ShaderManager->UpdateGlobalBuffer(ShaderKeys::Sky,
        context, totalTime, (float)m_frameCount, camPos);
    m_ShaderManager->UpdateMatrixBuffer(ShaderKeys::Sky, context, skyModel, view, proj);
    m_ShaderManager->UpdateLightBuffer(ShaderKeys::Sky, context, m_Light.get());

    SkyBuffer skyData;
    m_ShaderManager->UpdateSkyBuffer(context, skyData);
    m_ShaderManager->SetShaders(ShaderKeys::Sky, context);
    m_ShaderManager->SetConstantBuffers(ShaderKeys::Sky, context);

    m_Sky->Render(context);
} // DrawSky


void RenderingEngine::DrawCloud(ID3D11DeviceContext* context,
    float totalTime, XMFLOAT3 camPos, XMMATRIX view, XMMATRIX proj)
{
    m_Renderer->SetWrapSampler(0);
    m_TexturesManager->PSSetShaderResources(context,
        ConstantHelper::NOISE_PATH, 0);
    m_TexturesManager->PSSetShaderResources(context,
        ConstantHelper::BLUE_NOISE_PATH, 1);

    m_ShaderManager->UpdateGlobalBuffer(ShaderKeys::Cloud,
        context, totalTime, (float)m_frameCount, camPos);

    CloudBuffer cloudData((float)ConstantHelper::cloudType);
    m_ShaderManager->UpdateCloudBuffer(context, cloudData);

    if (ConstantHelper::cloudType == ConstantHelper::CloudType::Default)
    {
        XMMATRIX cloudModel = XMMatrixScaling(1.5f, 1.5f, 1.5f)
            * XMMatrixTranslation(camPos.x, camPos.y, camPos.z);
        m_ShaderManager->UpdateMatrixBuffer(ShaderKeys::Cloud,
            context, cloudModel, view, proj);
    }
    else
    {
        XMMATRIX cloudModel = XMMatrixTranslation(camPos.x, camPos.y, camPos.z);
        m_ShaderManager->UpdateMatrixBuffer(ShaderKeys::Cloud, context, cloudModel, view, proj);
    }
    m_ShaderManager->SetShaders(ShaderKeys::Cloud, context);
    m_ShaderManager->SetConstantBuffers(ShaderKeys::Cloud, context);

    m_Cloud->Render(context);
} // DrawCloud


void RenderingEngine::ApplyBicubicUpscale(ID3D11DeviceContext* context)
{
    m_ShaderManager->SetShaders(ShaderKeys::Bicubic, context);
    m_Renderer->SetLowResolutionShaderResources(0);

    m_Renderer->SetWrapSampler(0);

    m_ShaderManager->UpdateMatrixBuffer(
        ShaderKeys::Bicubic, context,
        XMMatrixIdentity(), XMMatrixIdentity(), XMMatrixIdentity());

    m_Quad->Render(context);
} // ApplyBicubicUpscale


void RenderingEngine::ApplyLensFlare(ID3D11DeviceContext* context,
    const XMMATRIX& view, const XMMATRIX& proj, const XMFLOAT3& camPos)
{
    m_Renderer->SetAdditiveAlphaBlending();
    m_Renderer->SetDepthBuffer(false);

    m_ShaderManager->SetShaders(ShaderKeys::LensFlare, context);
    m_Renderer->SetBorderSampler(0);

    // 버퍼
    GhostBuffer ghostBuffer;
    XMVECTOR sunWorldPos = XMLoadFloat3(&ConstantHelper::LightPosition);
    XMVECTOR sunScreenPos = XMVector3Project(sunWorldPos, 0, 0,
                            ConstantHelper::SCREEN_WIDTH, ConstantHelper::SCREEN_HEIGHT, 0, 1,
                            proj, view, XMMatrixIdentity());

    ghostBuffer.sunUV.x = XMVectorGetX(sunScreenPos) / (float)ConstantHelper::SCREEN_WIDTH;
    ghostBuffer.sunUV.y = XMVectorGetY(sunScreenPos) / (float)ConstantHelper::SCREEN_HEIGHT;

    m_ShaderManager->UpdateLensFlareBuffer(context, ghostBuffer);
    m_ShaderManager->UpdateMatrixBuffer(ShaderKeys::LensFlare, context,
        XMMatrixIdentity(), XMMatrixIdentity(), XMMatrixIdentity());
    m_ShaderManager->SetConstantBuffers(ShaderKeys::LensFlare, context);

    // 리소스
    m_Renderer->SetLowResolutionShaderResources(0); // t0: Scene
    m_TexturesManager->PSSetShaderResources(context, ConstantHelper::GHOST, 1);
    m_TexturesManager->PSSetShaderResources(context, ConstantHelper::GLOW, 2);
    m_TexturesManager->PSSetShaderResources(context, ConstantHelper::HALO1, 3);
	m_TexturesManager->PSSetShaderResources(context, ConstantHelper::HALO2, 4);
	m_TexturesManager->PSSetShaderResources(context, ConstantHelper::HALO3, 5);
    m_TexturesManager->PSSetShaderResources(context, ConstantHelper::STAR, 6);
    m_Renderer->SetMainDepthShaderResource(7);

    m_Quad->Render(context);

    m_Renderer->ClearShaderResources(0);
    m_Renderer->ClearShaderResources(1);
    m_Renderer->ClearShaderResources(4);

    m_Renderer->SetAlphaBlending(true);
} // ApplyLensFlare