#include "RenderStateWidget.h"
#include "imgui.h"


using namespace ImGui;
using namespace PropertyHelper;


RenderStateWidget::RenderStateWidget(
    const std::string & name,
    Property<bool> wireProp,
    Property<bool> backProp,
    Property<bool> depthProp)
    : IWidget(name),
    m_wireProp(wireProp),
    m_backProp(backProp),
    m_depthProp(depthProp)
{
} // RenderStateWidget


void RenderStateWidget::Frame()
{
    Begin(i_name.c_str());

    bool wire = m_wireProp.Get();
    bool back = m_backProp.Get();
    bool depth = m_depthProp.Get();

    if (Checkbox("Wireframe", &wire)) m_wireProp.Set(wire);
    if (Checkbox("BackCull", &back))  m_backProp.Set(back);
    if (Checkbox("DepthEnable", &depth)) m_depthProp.Set(depth);

    Separator();
    if (ImGui::Button("Reset"))
    {
        m_wireProp.Set(false);
        m_backProp.Set(false);
        m_depthProp.Set(true);
    }

    End();
} // RenderStateWidget