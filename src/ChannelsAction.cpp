#include "ChannelsAction.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>
#include <QLabel>

ChannelsAction::ChannelsAction(DimensionsViewerPlugin* dimensionsViewerPlugin) :
    PluginAction(dimensionsViewerPlugin, "Channels"),
    _enabled(this, "Enabled")
{
}

ChannelsAction::Widget::Widget(QWidget* parent, ChannelsAction* channelsAction) :
    WidgetAction::Widget(parent, channelsAction),
    _layout()
{
    setLayout(&_layout);

    _layout.addWidget(new QGroupBox("Channels"));
}