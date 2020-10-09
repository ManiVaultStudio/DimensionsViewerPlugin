#include "ConfigurationsModel.h"
#include "DimensionsViewerPlugin.h"

#include "PointData.h"

ConfigurationsModel::ConfigurationsModel(DimensionsViewerPlugin* dimensionsViewerPlugin) :
	QAbstractListModel(static_cast<QObject*>(dimensionsViewerPlugin)),
	_dimensionsViewerPlugin(dimensionsViewerPlugin),
	_configurations(),
	_selectionModel(this)
{
}

int ConfigurationsModel::rowCount(const QModelIndex& parentIndex /*= QModelIndex()*/) const
{
	return _configurations.count();
}

QVariant ConfigurationsModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
	return QVariant();
}

void ConfigurationsModel::addDataset(const QString& datasetName)
{
	beginInsertRows(QModelIndex(), _configurations.size(), _configurations.size());
	{
		_configurations << new Configuration(datasetName);
	}
	endInsertRows();
}

QStringList ConfigurationsModel::getConfigurationNames()
{
	QStringList configurationNames;

	for (const auto& configuration : _configurations)
		configurationNames << configuration->getChannelDatasetName(0, Qt::EditRole).toString();
	
	return configurationNames;
}

/*
void ConfigurationsModel::addDataset(const QString& datasetName)
{
	auto points = &_dimensionsViewerPlugin->getCore()->requestData<Points>(datasetName);

	if (points->indices.empty()) {
		_configurations << new Configuration(_dimensionsViewerPlugin, datasetName);
	}
	else {
		for (auto configuration : _configurations) {
			if (configuration->isSubset(datasetName))
				configuration->addSubset(datasetName);
		}
	}
}
*/