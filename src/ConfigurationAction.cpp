#include "ConfigurationAction.h"
#include "ChannelAction.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>
#include <QLabel>

using namespace hdps;
using namespace hdps::gui;

ConfigurationAction::ConfigurationAction(DimensionsViewerPlugin* dimensionsViewerPlugin) :
    PluginAction(dimensionsViewerPlugin, "Configuration"),
    hdps::EventListener(),
    _dimensionsAction(this),
    _channelsAction(this),
    _spec()
{
    setEventCore(_dimensionsViewerPlugin->getCore());

    _spec["modified"]           = 0;
    _spec["showDimensionNames"] = true;

    connect(&_dimensionsAction.getShowNamesAction(), &ToggleAction::toggled, [this](bool state) {
        _spec["modified"] = _spec["modified"].toInt() + 1;
        _spec["showDimensionNames"] = state;
    });
}

QVariantMap ConfigurationAction::getSpec()
{
    QVariantMap channels;

    for (auto channel : _channelsAction.getChannels()) {
        const auto channelSpec = channel->getSpec();

        if (channel->canDisplaySpec())
            channels[channel->getInternalName()] = channelSpec;
    }

    _spec["channels"] = channels;

    return _spec;
}

ConfigurationAction::Widget::Widget(QWidget* parent, ConfigurationAction* configurationAction) :
    WidgetAction::Widget(parent, configurationAction, State::Standard)
{
    setAutoFillBackground(true);

    auto layout = new QVBoxLayout();

    setLayout(layout);

    layout->setMargin(3);
    layout->setSpacing(3);

    layout->addWidget(configurationAction->_dimensionsAction.createWidget(this));
    layout->addWidget(configurationAction->_channelsAction.createWidget(this));
}