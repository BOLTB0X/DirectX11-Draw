#pragma once
#include "IWidget.h"
// Commom
#include "PropertyHelper.h"


class RenderStateWidget : public IWidget {
public:
    RenderStateWidget(const std::string& ,
        PropertyHelper::Property<bool>,
        PropertyHelper::Property<bool>,
        PropertyHelper::Property<bool>);
    RenderStateWidget(const RenderStateWidget& other) = delete;
    
    virtual ~RenderStateWidget() = default;
    virtual void Frame() override;

private:
    PropertyHelper::Property<bool> m_wireProp, m_backProp, m_depthProp;
}; // RenderStateWidget