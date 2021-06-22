#include "PluginAction.h"

#include "DimensionsViewerPlugin.h"

using namespace hdps::gui;

PluginAction::PluginAction(DimensionsViewerPlugin* dimensionsViewerPlugin, const QString& title) :
    WidgetAction(reinterpret_cast<QObject*>(dimensionsViewerPlugin)),
    _dimensionsViewerPlugin(dimensionsViewerPlugin)
{
    _dimensionsViewerPlugin->addAction(this);

    setText(title);
    setToolTip(title);
}