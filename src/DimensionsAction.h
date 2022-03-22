#pragma once

#include <actions/WidgetAction.h>
#include <actions/IntegralAction.h>
#include <actions/OptionAction.h>
#include <actions/ToggleAction.h>

using namespace hdps::gui;

class SettingsAction;

class DimensionsAction : public WidgetAction
{
protected:

    class Widget : public hdps::gui::WidgetActionWidget {
    public:
        Widget(QWidget* parent, DimensionsAction* dimensionsAction);
    };

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new DimensionsAction::Widget(parent, this);
    };

public:
    DimensionsAction(SettingsAction& settingsAction);

public: // Action getters

    IntegralAction& getSelectionCenterIndexAction() { return _selectionCenterIndexAction; }
    OptionAction& getSelectionCenterNameAction() { return _selectionCenterNameAction; }
    IntegralAction& getSelectionRadiusAction() { return _selectionRadiusAction; }
    ToggleAction& getShowNamesAction() { return _showNamesAction; }

protected:
    SettingsAction&     _settingsAction;
    IntegralAction      _selectionCenterIndexAction;
    OptionAction        _selectionCenterNameAction;
    IntegralAction      _selectionRadiusAction;
    ToggleAction        _showNamesAction;
    QStringList         _dimensionNames;
};