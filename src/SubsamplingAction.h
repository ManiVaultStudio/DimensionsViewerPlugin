#pragma once

#include "PluginAction.h"

#include "event/EventListener.h"

class ConfigurationAction;

class SubsamplingAction : public PluginAction, public hdps::EventListener
{
protected:

    class Widget : public hdps::gui::WidgetActionWidget {
    public:
        Widget(QWidget* parent, SubsamplingAction* subsamplingAction);
    };

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new SubsamplingAction::Widget(parent, this);
    };

public:
    SubsamplingAction(ConfigurationAction* configurationAction);

    hdps::gui::DecimalAction& getSubsamplingFactorAction() { return _subsamplingFactorAction; }

    bool shouldSubsample() const;
    double getNormalizedFactor() const;

protected:
    hdps::gui::DecimalAction    _subsamplingFactorAction;
    hdps::gui::TriggerAction    _subsamplingFactorOneOverEighthAction;
    hdps::gui::TriggerAction    _subsamplingFactorOneOverFourAction;
    hdps::gui::TriggerAction    _subsamplingFactorOneOverTwoAction;
};