#include "DimensionsViewerPlugin.h"
#include "DimensionsViewerWidget.h"
#include "SettingsWidget.h"
#include "Channel.h"

#include <QDebug>

Q_PLUGIN_METADATA(IID "nl.tudelft.DimensionsViewerPlugin")

DimensionsViewerPlugin::DimensionsViewerPlugin() : 
	ViewPlugin("Dimensions Viewer"),
	_configurationsModel(this),
	_dimensionsViewerWidget(),
	_settingsWidget()
{
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

	Channel::dimensionsViewerPlugin = this;

	_dimensionsViewerWidget = new DimensionsViewerWidget(this);
	_settingsWidget = new SettingsWidget(this);
}

void DimensionsViewerPlugin::init()
{
	addWidget(_dimensionsViewerWidget);
	addWidget(_settingsWidget);
}

void DimensionsViewerPlugin::dataAdded(const QString dataset)
{
	_configurationsModel.addDataset(dataset);
}

void DimensionsViewerPlugin::dataChanged(const QString dataset)
{
}

void DimensionsViewerPlugin::dataRemoved(const QString dataset)
{
}

void DimensionsViewerPlugin::selectionChanged(const QString dataName)
{
	emit pointsSelectionChanged(dataName);
}

hdps::DataTypes DimensionsViewerPlugin::supportedDataTypes() const
{
	hdps::DataTypes supportedTypes;

	supportedTypes.append(PointType);

	return supportedTypes;
}

DimensionsViewerPlugin* DimensionsViewerPluginFactory::produce()
{
    return new DimensionsViewerPlugin();
}