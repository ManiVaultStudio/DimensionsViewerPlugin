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
	_selectionModel(this),
    _datasetNames()
{
}

int ConfigurationsModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    ModelItem *parentItem;

    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = const_cast<Configurations*>(&_configurations);
    else
        parentItem = static_cast<ModelItem*>(parent.internalPointer());

    return parentItem->getChildCount();
}

int ConfigurationsModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    return 25;
    if (parent.isValid())
        return static_cast<ModelItem*>(parent.internalPointer())->columnCount();
    
    return _configurations.columnCount();
}

QVariant ConfigurationsModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    const auto item = index.parent().isValid() ? getItem(index) : &_configurations;

    return getItem(index)->getData(index, role);
}

bool ConfigurationsModel::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
{
    if (index == QModelIndex())
        return false;

    auto item = getItem(index);

    const auto affectedIndices = item->setData(index, value, role);

    for (auto affectedIndex : affectedIndices) {
        emit dataChanged(affectedIndex, affectedIndex);
    }

    return true;
}

bool ConfigurationsModel::setData(const std::int32_t& column, const QVariant& value, int role /*= Qt::EditRole*/)
{
	const auto selectedRows = _selectionModel.selectedRows();

	if (selectedRows.isEmpty())
		return false;

	setData(selectedRows.first().siblingAtColumn(column), value, role);

	return true;
}

Qt::ItemFlags ConfigurationsModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return getItem(index)->getFlags(index);
}

QVariant ConfigurationsModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0) {
        return "Name";
    }

    return QVariant();
}

QModelIndex ConfigurationsModel::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ModelItem* parentItem = nullptr;

    if (!parent.isValid())
        parentItem = const_cast<Configurations*>(&_configurations);
    else
        parentItem = static_cast<ModelItem*>(parent.internalPointer());

    auto childItem = parentItem->getChild(row);

    if (childItem)
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex ConfigurationsModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    auto childItem  = getItem(index);
    auto parentItem = childItem->getParent();

    if (parentItem == &_configurations || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->getChildIndex(), 0, parentItem);
}

void ConfigurationsModel::addDataset(const QString& datasetName)
{
    const auto dataName = _dimensionsViewerPlugin->getCore()->requestData<Points>(datasetName).getDataName();
    const auto hits     = match(index(0, to_ul(Configuration::Column::DataName)), Qt::DisplayRole, dataName, -1, Qt::MatchExactly | Qt::MatchRecursive);

    if (hits.isEmpty()) {
        const auto noConfigurations = _configurations.getChildCount();

        beginInsertRows(QModelIndex(), noConfigurations, noConfigurations);
        {
            _configurations.add(datasetName, dataName);
        }
        endInsertRows();
        
        _datasetNames << datasetName;

        for (auto configuration : _configurations._configurations) {
            const auto configurationIndex   = index(configuration->_index, 0);
            const auto channelsIndex        = index(0, 0, configurationIndex);
            const auto firstChannelIndex    = index(0, 0, channelsIndex);

            setData(firstChannelIndex.siblingAtColumn(to_ul(Channel::Column::DatasetNames)), _datasetNames);
        }

        if (_configurations.getChildCount() == 1)
            selectRow(0);

    } else {
        const auto configurationIndex   = hits.first().siblingAtColumn(0);
        const auto channelsIndex        = index(0, 0, configurationIndex);

        QVector<std::int32_t> channels;

        channels << to_ul(Channels::Row::Subset1);
        channels << to_ul(Channels::Row::Subset2);
        channels << to_ul(Channels::Row::Differential);

        for (auto channel : channels) {
            const auto channelIndex         = index(channel, 0, channelsIndex);
            const auto datasetNamesIndex    = channelIndex.siblingAtColumn(to_ul(Channel::Column::DatasetNames));

            auto datasetNames = datasetNamesIndex.data(Qt::EditRole).toStringList();

            datasetNames << datasetName;

            setData(datasetNamesIndex, datasetNames);
        }
    } 
}

void ConfigurationsModel::selectRow(const std::int32_t& row)
{
    const auto presentWarning = [](const QString& reason) {
        QMessageBox::warning(nullptr, "Unable to visualize dataset dimensions", reason);
    };

    try
    {
        const auto configurationIndex   = index(row, 0);
        const auto channelsIndex        = index(0, 0, configurationIndex);
        const auto firstChannelIndex    = index(0, 0, channelsIndex);

        if (firstChannelIndex.siblingAtColumn(to_ul(Channel::Column::NoDimensions)).data(Qt::EditRole).toInt() > Configuration::maxNoDimensions) {
            const auto datasetName = configurationIndex.siblingAtColumn(to_ul(Configuration::Column::DatasetName)).data(Qt::EditRole).toString();
            throw std::runtime_error(QString("%1 has more than %2 dimensions").arg(datasetName, QString::number(Configuration::maxNoDimensions)).toLatin1());
        }
        else {
            _selectionModel.select(configurationIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
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

void ConfigurationsModel::selectRow(const QString& datasetName)
{
    const auto dataName = _dimensionsViewerPlugin->getCore()->requestData<Points>(datasetName).getDataName();
    const auto hits     = match(index(0, to_ul(Configuration::Column::DataName)), Qt::DisplayRole, dataName, -1, Qt::MatchExactly);

    if (!hits.isEmpty())
        selectRow(hits.first().row());
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

const Configurations& ConfigurationsModel::getConfigurations() const
{
    return _configurations;
}

ModelItem* ConfigurationsModel::getItem(const QModelIndex& index) const
{
    if (index.isValid()) {
        auto modelItem = static_cast<ModelItem*>(index.internalPointer());

        if (modelItem)
            return modelItem;
    }

    return static_cast<ModelItem*>(const_cast<Configurations*>(&_configurations));
}