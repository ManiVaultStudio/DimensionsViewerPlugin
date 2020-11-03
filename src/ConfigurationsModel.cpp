#include "ConfigurationsModel.h"
#include "DimensionsViewerPlugin.h"

#include "PointData.h"

#include <QDebug>
#include <QMessageBox>

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

int ConfigurationsModel::columnCount(const QModelIndex& parentIndex /*= QModelIndex()*/) const
{
	return static_cast<int>(Configuration::Column::End) + 1;
}

QVariant ConfigurationsModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	return getConfiguration(index)->getData(index, role);
}

bool ConfigurationsModel::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
{
	auto configuration = getConfiguration(index);

	const auto affectedIndices = configuration->setData(index, value, role);

	for (auto affectedIndex : affectedIndices) {
		emit dataChanged(affectedIndex, affectedIndex);
	}

	return true;
}

bool ConfigurationsModel::setData(const std::int32_t& column, const QVariant& value, int role /*= Qt::EditRole*/)
{
	const auto selectedRows = _dimensionsViewerPlugin->getConfigurationsModel().getSelectionModel().selectedRows();

	if (selectedRows.isEmpty())
		return false;

	setData(selectedRows.first().siblingAtColumn(column), value, role);

	return true;
}

Qt::ItemFlags ConfigurationsModel::flags(const QModelIndex& index) const
{
	return _configurations[index.row()]->getFlags(index);
}

QVariant ConfigurationsModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (orientation == Qt::Horizontal) {
		switch (role)
		{
			case Qt::DecorationRole:
				break;

			case Qt::DisplayRole:
				return Configuration::getColumnName(section);

			case Qt::ToolTipRole:
				break;

			default:
				break;
		}
	}

	return QVariant();
}

void ConfigurationsModel::addDataset(const QString& datasetName)
{
    auto dataName = _dimensionsViewerPlugin->getCore()->requestData<Points>(datasetName).getDataName();

    //TODO
	//Configuration* parentConfiguration = false;

	//for (auto configuration : _configurations) {
	//	if (dataName == configuration->getChannelDataName(0, Qt::EditRole).toString()) {
	//		parentConfiguration = configuration;
	//		break;
	//	}
	//}

	//if (parentConfiguration) {
	//	const auto configurationIndex	= _configurations.indexOf(parentConfiguration);
	//	const auto subsetsIndex			= index(configurationIndex, static_cast<int>(Configuration::Column::Subsets));

	//	setData(subsetsIndex, parentConfiguration->getSubsets(Qt::EditRole).toStringList() << datasetName);
	//}
	//else {
		beginInsertRows(QModelIndex(), _configurations.size(), _configurations.size());
		{
			_configurations << new Configuration(_dimensionsViewerPlugin, datasetName, dataName);
		}
		endInsertRows();

		if (_configurations.size() == 1)
			selectRow(0);
	//}
}

QStringList ConfigurationsModel::getConfigurationNames()
{
	QStringList configurationNames;

	for (auto configuration : _configurations)
		configurationNames << configuration->getChannels()[0]->getDatasetName();
	
	return configurationNames;
}

void ConfigurationsModel::selectRow(const std::int32_t& rowIndex)
{
    const auto presentWarning = [](const QString& reason) {
        QMessageBox::warning(nullptr, "Unable to visualize dataset dimensions", reason);
    };

    try
    {
        auto configuration = _configurations[rowIndex];

        const auto datasetName = configuration->getChannels()[0]->getDatasetName();

        if (configuration->getChannels()[0]->getNoDimensions() > Configuration::maxNoDimensions) {
            throw std::runtime_error(QString("%1 has more than %2 dimensions").arg(datasetName, QString::number(Configuration::maxNoDimensions)).toLatin1());
        }
        else {
            _selectionModel.select(index(rowIndex), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        }
    }
    catch (std::exception e)
    {
        presentWarning(e.what());
    }
    catch (...) {
        presentWarning("An unhandled exception occurred");
    }
}

Configuration* ConfigurationsModel::getConfiguration(const QModelIndex& index) const
{
	try
	{
		if (index.isValid()) {
			return _configurations[index.row()];
		}
	}
	catch (std::exception exception)
	{
		return nullptr;
	}

	return nullptr;
}

Configuration* ConfigurationsModel::getSelectedConfiguration() const
{
	const auto selectedRows = _selectionModel.selectedRows();

	if (selectedRows.isEmpty())
		return nullptr;

	return getConfiguration(selectedRows.first());
}