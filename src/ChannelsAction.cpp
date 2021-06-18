#include "ChannelsAction.h"
#include "ConfigurationAction.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

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
    
    const auto update = [this, configurationAction]() -> void {
        QStringList datasetNames;

        for (auto datasetName : _dimensionsViewerPlugin->getCore()->requestAllDataNames(std::vector<DataType>({ PointType })))
            datasetNames << datasetName;

        for (auto channel : _channels) {
            channel->getDatasetName1Action().setOptions(datasetNames);
            channel->getDatasetName2Action().setOptions(datasetNames);
        }

        auto& dimensionsAction = configurationAction->getDimensionsAction();

        const auto numDimensions    = _channels.first()->getNumDimensions();
        const auto updateDuringDrag = _channels.first()->getNumPoints() < 100000;

        dimensionsAction.getSelectionCenterIndexAction().setUpdateDuringDrag(updateDuringDrag);
        dimensionsAction.getSelectionCenterIndexAction().setMaximum(numDimensions - 1);
        dimensionsAction.getSelectionCenterIndexAction().setValue(static_cast<std::int32_t>(floorf(static_cast<float>(numDimensions) / 2.0f)));
        dimensionsAction.getSelectionCenterNameAction().setOptions(_channels.first()->getDimensionNames());
        dimensionsAction.getSelectionRadiusAction().setUpdateDuringDrag(updateDuringDrag);
    };

    registerDataEventByType(PointType, [this, update](hdps::DataEvent* dataEvent) {
        if (dataEvent->getType() == EventType::DataAdded)
            update();

        setEnabled(true);
    });

    connect(&_channels.first()->getDatasetName1Action(), &OptionAction::currentTextChanged, [this, update](const QString& currentText) {
        update();
    });

    update();

    for (auto channel : _channels) {
        connect(&channel->getEnabledAction(), &ToggleAction::toggled, [this, configurationAction](bool state) {
            configurationAction->getDimensionsAction().setEnabled(canDisplay());
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
    WidgetAction::Widget(parent, channelsAction, State::Standard)
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