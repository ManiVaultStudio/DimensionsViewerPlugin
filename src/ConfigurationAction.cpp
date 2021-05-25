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
    _showAdvancedSettings(this, "Advanced settings"),
    _showDimensionNames(this, "Show dimension names")
{
    setEventCore(_dimensionsViewerPlugin->getCore());

    _channels << new ChannelAction(this, ChannelAction::ProfileType::Mean, false);
    _channels << new ChannelAction(this, ChannelAction::ProfileType::Mean, true);
    _channels << new ChannelAction(this, ChannelAction::ProfileType::Mean, true);
    _channels << new ChannelAction(this, ChannelAction::ProfileType::Differential, true);

    _showAdvancedSettings.setCheckable(true);
    _showAdvancedSettings.setChecked(true);

    _showDimensionNames.setCheckable(true);
    _showDimensionNames.setChecked(true);

    registerDataEventByType(PointType, [this](hdps::DataEvent* dataEvent) {
        if (dataEvent->getType() == EventType::DataAdded) {
            const auto dataSetName = static_cast<DataAddedEvent*>(dataEvent)->dataSetName;

            QStringList datasetNames;

            for (auto datasetName : _dimensionsViewerPlugin->getCore()->requestAllDataNames(std::vector<DataType>({ PointType })))
                datasetNames << datasetName;

            //qDebug() << datasetNames;
            auto& datasetNameAction = _channels[0]->getDatasetName1Action();

            datasetNameAction.setOptions(datasetNames);
            
            //if (!datasetNames.count() == 1)
                //datasetNameAction.setCurrentIndex(0);
        }
    });

    connect(&_channels[0]->getDatasetName1Action(), &OptionAction::currentTextChanged, [this](const QString& currentText) {
        auto datasetNames = _channels[0]->getDatasetName1Action().getOptions();

        datasetNames.removeOne(_channels[0]->getDatasetName1Action().getCurrentText());

        for (auto datasetName : datasetNames) {
            auto& points = dynamic_cast<Points&>(_dimensionsViewerPlugin->getCore()->requestData(datasetName));

            if (points.getNumDimensions() != _channels[0]->getNoDimensions())
                datasetNames.removeOne(datasetName);
        }

        for (int channelIndex = 1; channelIndex < getNumChannels(); channelIndex++)
            _channels[channelIndex]->getDatasetName1Action().setOptions(datasetNames);
    });
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

    const auto addChannels = [this, configurationAction]() {
        for (auto channel : configurationAction->_channels)
            _channelsGroupBoxLayout.addWidget(channel->createWidget(this));
    };

    addChannels();

    _miscellaneousGroupBox.setLayout(&_miscellaneousGroupBoxLayout);

    _miscellaneousGroupBoxLayout.addWidget(new StandardAction::CheckBox(this, &configurationAction->_showAdvancedSettings));
    _miscellaneousGroupBoxLayout.addWidget(new StandardAction::CheckBox(this, &configurationAction->_showDimensionNames));
    _miscellaneousGroupBoxLayout.addStretch(1);
}