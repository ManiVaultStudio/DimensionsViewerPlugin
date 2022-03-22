#pragma once

#include <actions/WidgetAction.h>
#include <actions/DecimalAction.h>
#include <actions/TriggerAction.h>

using namespace hdps::gui;

class SettingsAction;

class SubsamplingAction : public WidgetAction
{
protected:

    class Widget : public WidgetActionWidget {
    public:
        Widget(QWidget* parent, SubsamplingAction* subsamplingAction);
    };

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new SubsamplingAction::Widget(parent, this);
    };

public:
    SubsamplingAction(SettingsAction& settingsAction);

    bool shouldSubsample() const;
    double getNormalizedFactor() const;

public: // Action getters

    DecimalAction& getSubsamplingFactorAction() { return _subsamplingFactorAction; }

protected:
    SettingsAction&     _settingsAction;
    DecimalAction       _subsamplingFactorAction;
    TriggerAction       _subsamplingFactorOneOverEighthAction;
    TriggerAction       _subsamplingFactorOneOverFourAction;
    TriggerAction       _subsamplingFactorOneOverTwoAction;
};