#pragma once

#include "actions/Actions.h"

class DimensionsViewerPlugin;

class PluginAction : public hdps::gui::WidgetAction
{
public:
    PluginAction(DimensionsViewerPlugin* dimensionsViewerPlugin, const QString& title);

protected:
    DimensionsViewerPlugin*  _dimensionsViewerPlugin;
};