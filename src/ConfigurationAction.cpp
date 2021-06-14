#include "ConfigurationAction.h"
#include "ChannelAction.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

using namespace hdps;
using namespace hdps::gui;

ConfigurationAction::ConfigurationAction(DimensionsViewerPlugin* dimensionsViewerPlugin) :
    PluginAction(dimensionsViewerPlugin, "Configuration"),
    hdps::EventListener(),
    _channels(),
    _interactiveAction(this, "Interactive"),
    _showDimensionNamesAction(this, "Show dimension names"),
    _spec()
{
    setEventCore(_dimensionsViewerPlugin->getCore());

    _channels << new ChannelAction(this, ChannelAction::ProfileType::Mean, false);
    _channels << new ChannelAction(this, ChannelAction::ProfileType::Mean, true);
    _channels << new ChannelAction(this, ChannelAction::ProfileType::Mean, true);
    _channels << new ChannelAction(this, ChannelAction::ProfileType::Differential, true);
    
    _interactiveAction.setCheckable(true);
    _interactiveAction.setChecked(true);
    _interactiveAction.setToolTip("Whether to display all points or point selection");

    _showDimensionNamesAction.setCheckable(true);
    _showDimensionNamesAction.setChecked(true);

    const auto updateDatasetNames = [this]() -> void {
        QStringList datasetNames;

        for (auto datasetName : _dimensionsViewerPlugin->getCore()->requestAllDataNames(std::vector<DataType>({ PointType })))
            datasetNames << datasetName;

        for (auto channel : _channels) {
            channel->getDatasetName1Action().setOptions(datasetNames);
            channel->getDatasetName2Action().setOptions(datasetNames);
        }
    };

    registerDataEventByType(PointType, [this, updateDatasetNames](hdps::DataEvent* dataEvent) {
        if (dataEvent->getType() == EventType::DataAdded)
            updateDatasetNames();
    });

    connect(&_channels[0]->getDatasetName1Action(), &OptionAction::currentTextChanged, [this, updateDatasetNames](const QString& currentText) {
        updateDatasetNames();
    });

    updateDatasetNames();

    _spec["modified"]           = 0;
    _spec["showDimensionNames"] = true;

    connect(&_showDimensionNamesAction, &ToggleAction::toggled, [this](bool state) {
        _spec["modified"] = _spec["modified"].toInt() + 1;
        _spec["showDimensionNames"] = state;
    });
}

QVariantMap ConfigurationAction::getSpec()
{
    QVariantMap channels;

    for (auto channel : _channels) {
        const auto channelSpec = channel->getSpec();

        if (channel->canDisplaySpec())
            channels[channel->getInternalName()] = channelSpec;
    }

    _spec["channels"] = channels;

    return _spec;
}

ConfigurationAction::Widget::Widget(QWidget* parent, ConfigurationAction* configurationAction) :
    WidgetAction::Widget(parent, configurationAction, State::Standard),
    _layout(),
    _channelsGroupBox("Channels"),
    _channelsGroupBoxLayout(),
    _miscellaneousGroupBox("Miscellaneous"),
    _miscellaneousGroupBoxLayout()
{
    
    setAutoFillBackground(true);
    setLayout(&_layout);

    _layout.setMargin(4);

    _layout.addWidget(&_channelsGroupBox);
    _layout.addWidget(&_miscellaneousGroupBox);

    _channelsGroupBox.setLayout(&_channelsGroupBoxLayout);

    _channelsGroupBoxLayout.setSpacing(4);

    const auto addChannels = [this, configurationAction]() {
        for (auto channel : configurationAction->_channels)
            _channelsGroupBoxLayout.addWidget(channel->createWidget(this));
    };

    addChannels();

    _miscellaneousGroupBox.setLayout(&_miscellaneousGroupBoxLayout);

    auto interactiveWidget          = configurationAction->_interactiveAction.createWidget(this);
    auto showDimensionNamesWidget   = configurationAction->_showDimensionNamesAction.createWidget(this);

    _miscellaneousGroupBoxLayout.addWidget(interactiveWidget);
    _miscellaneousGroupBoxLayout.addWidget(showDimensionNamesWidget);
    _miscellaneousGroupBoxLayout.addStretch(1);
}