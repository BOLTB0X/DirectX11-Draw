#include "Pch.h"
#include "StatsWidget.h"
// Imgui
#include "imgui.h"


using namespace PropertyHelper;
using namespace ImGui;


StatsWidget::StatsWidget(
    const std::string& name,
    Property<float> timeProp,
    Property<int> fpsProp)
    : IWidget(name),
    m_timeProp(timeProp),
    m_fpsProp(fpsProp)
{
} // StatsWidget


void StatsWidget::Frame()
{
    Begin(i_name.c_str());

    Text("Average: %.3f ms/frame (%.1f FPS)",
        1000.0f / GetIO().Framerate, GetIO().Framerate);

    Separator();

    Text("Total Time: %.2f s", m_timeProp.Get());
    Text("FPS: %d", m_fpsProp.Get());

    End();
} // Frame

//StatsWidget::StatsWidget(
//    const std::string& name, Timer* timer, Fps* fps)
//    : IWidget(name),
//    m_Timer(timer),
//    m_Fps(fps)
//{
//} // StatsWidget


//void StatsWidget::Frame()
//{
//    Begin(i_name.c_str());
//
//    Text("Average: %.3f ms/frame (%.1f FPS)",
//        1000.0f / GetIO().Framerate, GetIO().Framerate);
//
//    Separator();
//
//    if (m_Timer == nullptr || m_Fps == nullptr)
//        return;
//       
//    Text("Total Time: %.2f s", m_Timer->GetTotalTime());
//    Text("FPS: % d", m_Fps->GetFps());
//
//    End();
//} // Frame