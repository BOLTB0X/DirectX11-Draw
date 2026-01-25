#pragma once
#include "IWidget.h"
// Common
#include "PropertyHelper.h"

class Timer;
class Fps;


class StatsWidget : public IWidget {
public:
    //StatsWidget(const std::string& , Timer*, Fps*);
    StatsWidget(const std::string&,
        PropertyHelper::Property<float>,
        PropertyHelper::Property<int>);
    StatsWidget(const StatsWidget& other) = delete;
    
    virtual ~StatsWidget() = default;
    virtual void Frame() override;

private:
    PropertyHelper::Property<float> m_timeProp;
    PropertyHelper::Property<int> m_fpsProp;
    //Timer* m_Timer;
    //Fps* m_Fps;
}; // StatsWidget