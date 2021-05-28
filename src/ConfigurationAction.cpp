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
    _showAdvancedSettingsAction(this, "Advanced settings"),
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

    _showAdvancedSettingsAction.setCheckable(true);
    _showAdvancedSettingsAction.setChecked(false);

    _showDimensionNamesAction.setCheckable(true);
    _showDimensionNamesAction.setChecked(true);

    const auto updateDatasetNames = [this]() -> void {
        QStringList datasetNames;

        for (auto datasetName : _dimensionsViewerPlugin->getCore()->requestAllDataNames(std::vector<DataType>({ PointType })))
            datasetNames << datasetName;

        _channels[0]->getDatasetName1Action().setOptions(datasetNames);
        _channels[0]->getDatasetName2Action().setOptions(datasetNames);

        datasetNames.removeOne(_channels[0]->getDatasetName1Action().getCurrentText());

        for (auto datasetName : datasetNames) {
            auto& points = dynamic_cast<Points&>(_dimensionsViewerPlugin->getCore()->requestData(datasetName));

            if (points.getNumDimensions() != _channels[0]->getNoDimensions())
                datasetNames.removeOne(datasetName);
        }

        for (int channelIndex = 1; channelIndex < getNumChannels(); channelIndex++) {
            _channels[channelIndex]->getDatasetName1Action().setOptions(datasetNames);
            _channels[channelIndex]->getDatasetName2Action().setOptions(datasetNames);
        }

        //if (!datasetNames.count() == 1)
                //datasetNameAction.setCurrentIndex(0);
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
}

QVariantMap ConfigurationAction::getSpec()
{
    QVariantMap channels;

    for (auto channel : _channels) {
        if (!channel->canDisplaySpec())
            continue;

        const auto channelSpec = channel->getSpec();

        if (channelSpec["modified"] > _spec["modified"])
            _spec["modified"] = channelSpec["modified"];

        channels[channel->getInternalName()] = channelSpec;
    }

    _spec["channels"] = channels;

    return _spec;
}

ConfigurationAction::Widget::Widget(QWidget* parent, ConfigurationAction* configurationAction) :
    WidgetAction::Widget(parent, configurationAction),
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

    _miscellaneousGroupBoxLayout.addWidget(new StandardAction::CheckBox(this, &configurationAction->_interactiveAction));
    _miscellaneousGroupBoxLayout.addWidget(new StandardAction::CheckBox(this, &configurationAction->_showAdvancedSettingsAction));
    _miscellaneousGroupBoxLayout.addWidget(new StandardAction::CheckBox(this, &configurationAction->_showDimensionNamesAction));
    _miscellaneousGroupBoxLayout.addStretch(1);
}