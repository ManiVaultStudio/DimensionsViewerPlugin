#include "ConfigurationAction.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

ConfigurationAction::ConfigurationAction(DimensionsViewerPlugin* dimensionsViewerPlugin, const QString& datasetName) :
    PluginAction(dimensionsViewerPlugin, "Configuration"),
    _channels(dimensionsViewerPlugin),
    _miscellaneous(dimensionsViewerPlugin)
{
}

ConfigurationAction::Widget::Widget(QWidget* parent, ConfigurationAction* configurationAction) :
    WidgetAction::Widget(parent, configurationAction),
    _layout()
{
    setLayout(&_layout);

    _layout.addWidget(configurationAction->_channels.createWidget(this));
    _layout.addWidget(configurationAction->_miscellaneous.createWidget(this));
}