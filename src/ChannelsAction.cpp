#include "ChannelsAction.h"
#include "ConfigurationAction.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>
#include <QGroupBox>

using namespace hdps;
using namespace hdps::gui;

ChannelsAction::ChannelsAction(ConfigurationAction* configurationAction) :
    PluginAction(configurationAction->getDimensionsViewerPlugin(), "Configuration"),
    hdps::EventListener(),
    _configurationAction(configurationAction),
    _channels()
{
    setEnabled(false);
    setEventCore(_dimensionsViewerPlugin->getCore());

    _channels << new ChannelAction(this, "Channel 1", ChannelAction::ProfileType::Mean);
    _channels << new ChannelAction(this, "Channel 2", ChannelAction::ProfileType::Mean);
    _channels << new ChannelAction(this, "Channel 3", ChannelAction::ProfileType::Mean);
    _channels << new ChannelAction(this, "Channel 4", ChannelAction::ProfileType::Mean);

    /* TODO
    connect(&_channels.first()->getDatasetName1Action(), &OptionAction::modelChanged, [this]() {
        setEnabled(_channels.first()->getDatasetName1Action().hasOptions());
    });
    */

    for (auto channel : _channels) {
        connect(&channel->getEnabledAction(), &ToggleAction::toggled, [this, configurationAction](bool state) {
            configurationAction->getDimensionsAction().setEnabled(canDisplay());
            configurationAction->getSubsamplingAction().setEnabled(canDisplay());
        });
    }
}

bool ChannelsAction::canDisplay() const
{
    for (auto channel : _channels)
        if (channel->getEnabledAction().isChecked())
            return true;

    return false;
}

ChannelsAction::Widget::Widget(QWidget* parent, ChannelsAction* channelsAction) :
    WidgetActionWidget(parent, channelsAction, State::Standard)
{
    auto layout         = new QVBoxLayout();
    auto groupBox       = new QGroupBox("Channels");
    auto groupBoxLayout = new QVBoxLayout();
    
    setLayout(layout);

    layout->setMargin(2);

    layout->addWidget(groupBox);

    groupBox->setLayout(groupBoxLayout);

    groupBoxLayout->setMargin(9);
    groupBoxLayout->setSpacing(3);

    const auto addChannels = [this, channelsAction, groupBoxLayout]() {
        for (auto channel : channelsAction->_channels)
            groupBoxLayout->addWidget(channel->createWidget(this));
    };

    addChannels();
}