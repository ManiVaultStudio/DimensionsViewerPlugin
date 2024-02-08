#pragma once

#include <actions/WidgetAction.h>
#include <actions/GroupsAction.h>

using namespace mv;
using namespace mv::gui;

class SettingsAction;

class LayersAction : public GroupAction
{
public:
    LayersAction(SettingsAction& settingsAction);

public: // Action getters

    SettingsAction& getSettingsAction();

protected:
    SettingsAction&     _settingsAction;
};