#include "ConfigurationsModel.h"
#include "DimensionsViewerPlugin.h"
#include "Configurations.h"
#include "Configuration.h"

#include "PointData.h"

#include <QDebug>
#include <QMessageBox>

ConfigurationsModel::ConfigurationsModel(DimensionsViewerPlugin* dimensionsViewerPlugin) :
    QAbstractItemModel(static_cast<QObject*>(dimensionsViewerPlugin)),
	_dimensionsViewerPlugin(dimensionsViewerPlugin),
	_configurations(),
	_selectionModel(this)
{
}

int ConfigurationsModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    const auto parentItem = getItem(parent);

    return parentItem ? parentItem->getChildCount() : 0;
}

int ConfigurationsModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    Q_UNUSED(parent);
    return _configurations.getColumnCount();
}

QVariant ConfigurationsModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    auto item = getItem(index);

    return item->getData(index, role);
}

bool ConfigurationsModel::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
{
	const auto affectedIndices = _configurations.setData(index, value, role);

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
    return getItem(index)->getFlags(index);
}

QVariant ConfigurationsModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
    if (orientation == Qt::Horizontal) {
        return Configuration::getColumnName(static_cast<Configuration::Column>(section));
    }

    return QVariant();
}

QModelIndex ConfigurationsModel::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    auto parentItem = getItem(parent);

    if (!parentItem)
        return QModelIndex();

    auto childItem = parentItem->getChild(row);

    if (childItem)
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex ConfigurationsModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    auto childItem = getItem(index);
    auto parentItem = childItem ? childItem->getParent() : nullptr;

    if (parentItem == &_configurations || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->getChildCount(), 0, parentItem);
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
        const auto noConfigurations = _configurations.getChildCount();

		beginInsertRows(QModelIndex(), noConfigurations, noConfigurations);
		{
            _configurations.add(datasetName, dataName);
		}
		endInsertRows();

		if (_configurations.getChildCount() == 1)
			selectRow(0);
	//}
}

void ConfigurationsModel::selectRow(const std::int32_t& rowIndex)
{
    const auto presentWarning = [](const QString& reason) {
        QMessageBox::warning(nullptr, "Unable to visualize dataset dimensions", reason);
    };

    try
    {
        _selectionModel.select(index(rowIndex, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

        /* TODO
        auto configuration = _configurations.getChild(rowIndex);

        const auto datasetName = configuration->getChannels()[0]->getDatasetName();

        if (configuration->getChannels()[0]->getNoDimensions() > Configuration::maxNoDimensions) {
            throw std::runtime_error(QString("%1 has more than %2 dimensions").arg(datasetName, QString::number(Configuration::maxNoDimensions)).toLatin1());
        }
        else {
            _selectionModel.select(index(rowIndex, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        }*/
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
    return reinterpret_cast<Configuration*>(_configurations.getChild(index.row()));
}

Configuration* ConfigurationsModel::getSelectedConfiguration() const
{
	const auto selectedRows = _selectionModel.selectedRows();

	if (selectedRows.isEmpty())
		return nullptr;

	return getConfiguration(selectedRows.first());
}

ModelItem* ConfigurationsModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        auto modelItem = static_cast<ModelItem*>(index.internalPointer());

        if (modelItem)
            return modelItem;
    }

    return static_cast<ModelItem*>(const_cast<Configurations*>(&_configurations));
}