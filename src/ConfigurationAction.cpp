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
    _subsamplingAction(this),
    _channelsAction(this),
    _spec(),
    _isLoading(false)
{
    setEventCore(_dimensionsViewerPlugin->getCore());

    _spec["modified"]           = 0;
    _spec["showDimensionNames"] = true;

    connect(&_dimensionsAction.getShowNamesAction(), &ToggleAction::toggled, [this](bool state) {
        _spec["modified"] = _spec["modified"].toInt() + 1;
        _spec["showDimensionNames"] = state;
    });

    registerDataEventByType(PointType, [this](hdps::DataEvent* dataEvent) {
        if (dataEvent->getType() == EventType::DataAdded)
            updateSecondaryDatasetNames();
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

void ConfigurationAction::updateSecondaryDatasetNames()
{
    auto primaryChannel = _channelsAction.getChannels().first();

    const auto numDimensions = primaryChannel->getNumDimensions();

    QStringList datasetNames;

    for (auto candidateDatasetName : _dimensionsViewerPlugin->getCore()->requestAllDataNames(std::vector<DataType>({ PointType }))) {
        if (getNumDimensions(candidateDatasetName) == numDimensions)
            datasetNames << candidateDatasetName;
    }

    for (auto channel : _channelsAction.getChannels()) {
        if (channel->getIndex() == 0)
            continue;

        channel->getDatasetName1Action().setOptions(datasetNames);
        channel->getDatasetName2Action().setOptions(datasetNames);
    }
}

void ConfigurationAction::loadDataset(const QString& datasetName)
{
    _isLoading = true;

    auto primaryChannel = _channelsAction.getChannels().first();

    if (datasetName == primaryChannel->getDatasetName1Action().getCurrentText())
        return;

    primaryChannel->getDatasetName1Action().setOptions(QStringList() << datasetName);
    primaryChannel->getDatasetName2Action().setOptions(QStringList() << datasetName);
    primaryChannel->getDatasetName1Action().setCurrentText(datasetName);
    primaryChannel->getDatasetName2Action().setCurrentText(datasetName);

    for (auto channel : _channelsAction.getChannels())
        channel->getEnabledAction().setChecked(channel->getIndex() == 0);

    const auto numPoints        = primaryChannel->getNumPoints();
    const auto numDimensions    = primaryChannel->getNumDimensions();
    const auto updateDuringDrag = numPoints < 100000;

    updateSecondaryDatasetNames();
       
    _dimensionsAction.getSelectionCenterIndexAction().setUpdateDuringDrag(updateDuringDrag);
    _dimensionsAction.getSelectionCenterIndexAction().setMaximum(numDimensions - 1);
    _dimensionsAction.getSelectionCenterIndexAction().setValue(static_cast<std::int32_t>(floorf(static_cast<float>(numDimensions) / 2.0f)));
    _dimensionsAction.getSelectionCenterNameAction().setOptions(primaryChannel->getDimensionNames());
    _dimensionsAction.getSelectionRadiusAction().setUpdateDuringDrag(updateDuringDrag);

    _subsamplingAction.setChecked(numPoints > 100000);

    _isLoading = false;

    for (auto channel : _channelsAction.getChannels())
        channel->updateSpec();
}

QString ConfigurationAction::getLoadedDataset()
{
    return _channelsAction.getChannels().first()->getDatasetName1Action().getCurrentText();
}

bool ConfigurationAction::isLoading() const
{
    return _isLoading;
}

bool ConfigurationAction::isLoaded() const
{
    return false;// _channelsAction.getChannels().first()->isLoaded();
}

std::int32_t ConfigurationAction::getNumDimensions(const QString& datasetName) const
{
    return dynamic_cast<Points&>(DataSet::getSourceData(_dimensionsViewerPlugin->getCore()->requestData(datasetName))).getNumDimensions();
}

QStringList ConfigurationAction::getCompatibleDatasetNames(const QString& datasetName) const
{
    QStringList datasetNames;

    const auto numDimensions = getNumDimensions(datasetName);

    for (auto candidateDatasetName : _dimensionsViewerPlugin->getCore()->requestAllDataNames(std::vector<DataType>({ PointType }))) {
        if (getNumDimensions(candidateDatasetName) == numDimensions)
            datasetNames << candidateDatasetName;
    }

    return datasetNames;
}

ConfigurationAction::Widget::Widget(QWidget* parent, ConfigurationAction* configurationAction) :
    WidgetAction::Widget(parent, configurationAction, State::Standard)
{
    setAutoFillBackground(true);

    auto layout = new QVBoxLayout();

    setLayout(layout);

    layout->setMargin(3);
    layout->setSpacing(3);

    auto topLayout = new QHBoxLayout();

    topLayout->addWidget(configurationAction->_dimensionsAction.createWidget(this), 1);
    topLayout->addWidget(configurationAction->_subsamplingAction.createWidget(this));

    layout->addLayout(topLayout);
    layout->addWidget(configurationAction->_channelsAction.createWidget(this));
}