#pragma once

#include "PluginAction.h"

#include "event/EventListener.h"

class ConfigurationAction;

class MiscellaneousAction : public PluginAction, public hdps::EventListener
{
protected:

    class Widget : public PluginAction::Widget {
    public:
        Widget(QWidget* parent, MiscellaneousAction* miscellaneousAction);
    };

    QWidget* getWidget(QWidget* parent, const Widget::State& widgetType = Widget::State::Standard) override {
        return new MiscellaneousAction::Widget(parent, this);
    };

public:
    MiscellaneousAction(ConfigurationAction* configurationAction);

    hdps::gui::ToggleAction& getEnableSubsamplingAction() { return _enableSubsamplingAction; }
    hdps::gui::IntegralAction& getSubsamplingFactorAction() { return _subsamplingFactorAction; }

protected:
    hdps::gui::ToggleAction     _enableSubsamplingAction;
    hdps::gui::IntegralAction   _subsamplingFactorAction;
};