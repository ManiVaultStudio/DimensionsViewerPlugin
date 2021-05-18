#include "SettingsAction.h"
#include "DimensionsViewerPlugin.h"
#include "ConfigurationAction.h"

#include "PointData.h"

#include <QDebug>

using namespace hdps;
using namespace hdps::gui;

SettingsAction::SettingsAction(DimensionsViewerPlugin* dimensionsViewerPlugin) :
    PluginAction(dimensionsViewerPlugin, "Settings"),
    hdps::EventListener(),
    _dimensionsViewerPlugin(dimensionsViewerPlugin),
    _currentDataset(this, "Current"),
    _configurations()
{
    setEventCore(_dimensionsViewerPlugin->getCore());

    registerDataEventByType(PointType, [this](hdps::DataEvent* dataEvent) {
        if (dataEvent->getType() == EventType::DataAdded) {
            const auto dataSetName = static_cast<DataAddedEvent*>(dataEvent)->dataSetName;

            _configurations.insert(dataSetName, new ConfigurationAction(_dimensionsViewerPlugin, dataSetName));

            _currentDataset.setOptions(_configurations.keys());
        }
    });

    connect(&_currentDataset, &OptionAction::currentTextChanged, this, &SettingsAction::selectConfiguration);
}

void SettingsAction::selectConfiguration(const QString& datasetName)
{
    qDebug() << "selectConfiguration" << datasetName;
}

SettingsAction::Widget::Widget(QWidget* parent, SettingsAction* settingsAction) :
    WidgetAction::Widget(parent, settingsAction),
    _layout(),
    _currentDatasetWidget(this, &settingsAction->_currentDataset, false)
{
    setAutoFillBackground(true);

    _layout.addWidget(&_currentDatasetWidget);

    setLayout(&_layout);

    connect(&settingsAction->_currentDataset, &OptionAction::currentTextChanged, this, [this, settingsAction](const QString& datasetName) {
        const auto configurationItem = _layout.itemAt(1);

        if (configurationItem != nullptr)
            delete configurationItem->widget();

        _layout.addWidget(settingsAction->getConfiguration(datasetName)->createWidget(this));
    });
}