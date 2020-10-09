#include "DimensionsViewerPlugin.h"
#include "DimensionsViewerWidget.h"
#include "SettingsWidget.h"
#include "Channel.h"

#include "Application.h"

#include <QDebug>

Q_PLUGIN_METADATA(IID "nl.tudelft.DimensionsViewerPlugin")

DimensionsViewerPlugin::DimensionsViewerPlugin() : 
	ViewPlugin("Dimensions Viewer"),
	_configurationsModel(this),
	_channels(),
	_dimensionsViewerWidget(),
	_settingsWidget()
{
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

	_dimensionsViewerWidget = new DimensionsViewerWidget(this);
	_settingsWidget = new SettingsWidget(this);

	_channels.append(new Channel("Channel1", this));
	_channels.append(new Channel("Channel2", this));
	_channels.append(new Channel("Channel3", this));
}

void DimensionsViewerPlugin::init()
{
	addWidget(_dimensionsViewerWidget);
	addWidget(_settingsWidget);
}

void DimensionsViewerPlugin::dataAdded(const QString dataset)
{
	//qDebug() << "Data added" << dataset;

	_configurationsModel.addDataset(dataset);
}

void DimensionsViewerPlugin::dataChanged(const QString dataset)
{
	//qDebug() << "Data changed" << dataset;

	/*
	if (dataset != _settingsWidget->getCurrentDatasetName())
		return;
	*/

	//_channels.update(_points, selectedIndices());
}

void DimensionsViewerPlugin::dataRemoved(const QString dataset)
{
	//qDebug() << "Data removed" << dataset;
	
	//_dataSets.remove(dataset);
}

void DimensionsViewerPlugin::selectionChanged(const QString dataset)
{
	//qDebug() << "Selection changed" << dataset;

	/*
	if (dataset != _points->getDataName())
		return;
	*/

	//_channels.update(_points, selectedIndices());
}

hdps::DataTypes DimensionsViewerPlugin::supportedDataTypes() const
{
	hdps::DataTypes supportedTypes;

	supportedTypes.append(PointType);

	return supportedTypes;
}

Points* DimensionsViewerPlugin::getPoints(const QString& datasetName)
{
	return &_core->requestData<Points>(datasetName);
}

DimensionsViewerPlugin* DimensionsViewerPluginFactory::produce()
{
    return new DimensionsViewerPlugin();
}