#pragma once

#include "PluginAction.h"

#include "event/EventListener.h"

class ConfigurationAction;

class DimensionsAction : public PluginAction, public hdps::EventListener
{
protected:

    class Widget : public PluginAction::Widget {
    public:
        Widget(QWidget* parent, DimensionsAction* dimensionsAction);
    };

    QWidget* getWidget(QWidget* parent, const Widget::State& widgetType = Widget::State::Standard) override {
        return new DimensionsAction::Widget(parent, this);
    };

public:
    DimensionsAction(ConfigurationAction* configurationAction);

    hdps::gui::IntegralAction& getSelectionCenterIndexAction() { return _selectionCenterIndexAction; }
    hdps::gui::OptionAction& getSelectionCenterNameAction() { return _selectionCenterNameAction; }
    hdps::gui::IntegralAction& getSelectionRadiusAction() { return _selectionRadiusAction; }
    hdps::gui::ToggleAction& getShowNamesAction() { return _showNamesAction; }

protected:
    hdps::gui::IntegralAction   _selectionCenterIndexAction;
    hdps::gui::OptionAction     _selectionCenterNameAction;
    hdps::gui::IntegralAction   _selectionRadiusAction;
    hdps::gui::ToggleAction     _showNamesAction;
    QStringList                 _dimensionNames;
};