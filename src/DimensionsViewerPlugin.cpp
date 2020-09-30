#include "DimensionsViewerPlugin.h"
#include "DimensionsViewerWidget.h"
#include "SettingsWidget.h"
#include "Channel.h"

#include "Application.h"

#include <QDebug>

Q_PLUGIN_METADATA(IID "nl.tudelft.HistogramViewerPlugin")

DimensionsViewerPlugin::DimensionsViewerPlugin() : 
	ViewPlugin("Dimensions Viewer"),
	_datasets(),
	_dimensionsViewerWidget(new DimensionsViewerWidget(this)),
	_settingsWidget(new SettingsWidget(this)),
	_channels()
{
	setIcon(hdps::Application::getIconFont("FontAwesome").getIcon("chart-line"));
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	//setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

	/*
	QObject::connect(_settingsWidget, &SettingsWidget::datasetChanged, [this](const QString& dataset) {
		setCurrentDatasetName(dataset);
	});
	*/

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

	/*
	auto& points = dynamic_cast<Points&>(_core->requestData(dataset));

	const auto dataName = points.getDataName();

	auto isSubset = false;

	QString sourceDataSetName;

	for (const auto& datasetName : _datasets.keys()) {
		auto& localPoints = dynamic_cast<Points&>(_core->requestData(datasetName));

		const auto sourceDataName = localPoints.getDataName();

		if (dataName == sourceDataName) {
			isSubset = true;
			sourceDataSetName = localPoints.getName();
		}
	}

	if (isSubset) {
		_datasets[sourceDataSetName] << dataset;
	}
	else {
		_datasets[dataset] = QStringList();
	}

	emit datasetsChanged(_datasets);
	*/
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
	
	_datasets.remove(dataset);
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

DimensionsViewerPlugin* DimensionsViewerPluginFactory::produce()
{
    return new DimensionsViewerPlugin();
}