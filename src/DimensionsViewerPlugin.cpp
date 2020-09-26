#include "DimensionsViewerPlugin.h"
#include "DimensionsViewerWidget.h"
#include "SettingsWidget.h"

#include "Application.h"

#include <QDebug>

Q_PLUGIN_METADATA(IID "nl.tudelft.HistogramViewerPlugin")

DimensionsViewerPlugin::DimensionsViewerPlugin() : 
	ViewPlugin("Dimensions Viewer"),
	_datasetsModel(),
	_dimensionsViewerWidget(new DimensionsViewerWidget(this)),
	_settingsWidget(new SettingsWidget(this)),
	_dimensions(this),
	_points(nullptr)
{
	//setIcon(hdps::Application::getIconFont("FontAwesome").getIcon("chart-line"));

	QObject::connect(_settingsWidget, &SettingsWidget::datasetChanged, [this](const QString& dataset) {
		_points = &dynamic_cast<Points&>(_core->requestData(dataset));

		_dimensions.update(_points, selectedIndices());
	});
}

void DimensionsViewerPlugin::init()
{
	addWidget(_dimensionsViewerWidget);
	addWidget(_settingsWidget);
}

std::vector<std::uint32_t> DimensionsViewerPlugin::selectedIndices() const
{
	if (_points == nullptr)
		return std::vector<std::uint32_t>();

	const auto& selection = dynamic_cast<Points&>(_core->requestSelection(_points->getDataName()));

	return selection.indices;
}

QSize DimensionsViewerPlugin::sizeHint() const
{
	return QSize(100, 100);
}

void DimensionsViewerPlugin::dataAdded(const QString dataset)
{
	//qDebug() << "Data added" << dataset;

	auto datasets = _datasetsModel.stringList();

	datasets << dataset;

	_datasetsModel.setStringList(datasets);

	//_dimensions.update(dataset);
}

void DimensionsViewerPlugin::dataChanged(const QString dataset)
{
	//qDebug() << "Data changed" << dataset;

	if (_points == nullptr)
		return;

	if (dataset != _settingsWidget->getCurrentDataset())
		return;

	_dimensions.update(_points, selectedIndices());
}

void DimensionsViewerPlugin::dataRemoved(const QString dataset)
{
	//qDebug() << "Data removed" << dataset;
	
	auto datasets = _datasetsModel.stringList();

	datasets.removeOne(dataset);

	_datasetsModel.setStringList(datasets);
}

void DimensionsViewerPlugin::selectionChanged(const QString dataset)
{
	//qDebug() << "Selection changed" << dataset;

	if (_points == nullptr)
		return;

	if (dataset != _points->getDataName())
		return;

	_dimensions.update(_points, selectedIndices());
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