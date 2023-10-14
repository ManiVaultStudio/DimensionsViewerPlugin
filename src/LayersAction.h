#pragma once

#include <actions/WidgetAction.h>
#include <actions/GroupsAction.h>

using namespace mv;
using namespace mv::gui;

class SettingsAction;

class LayersAction : public WidgetAction
{
protected:

    class Widget : public mv::gui::WidgetActionWidget {
    public:
        Widget(QWidget* parent, LayersAction* layersAction);
    };

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new Widget(parent, this);
    };

public:
    LayersAction(SettingsAction& settingsAction);

public: // Action getters

    SettingsAction& getSettingsAction();

protected:
    SettingsAction&     _settingsAction;
};