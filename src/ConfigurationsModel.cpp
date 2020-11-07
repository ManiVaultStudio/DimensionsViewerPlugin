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
    return 20;//
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

    auto item = getItem(index);

    return item->getData(index, role);
}

bool ConfigurationsModel::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
{
    if (index == QModelIndex())
        return false;

    const auto affectedIndices = getItem(index)->setData(index, value, role);

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
    if (!index.isValid())
        return Qt::NoItemFlags;

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
    auto dataName = _dimensionsViewerPlugin->getCore()->requestData<Points>(datasetName).getDataName();

    const auto hits = match(index(0, Configuration::Column::DataName), Qt::DisplayRole, dataName, -1, Qt::MatchExactly | Qt::MatchRecursive);

    const auto configurationsIndex = index(0, Configurations::Column::Name);

    if (hits.isEmpty()) {
        const auto noConfigurations = _configurations.getChildCount();

        beginInsertRows(QModelIndex(), noConfigurations, noConfigurations);
        {
            _configurations.add(datasetName, dataName);
        }
        endInsertRows();

        /**/
        const auto datasetNamesIndex = configurationsIndex.siblingAtColumn(Configurations::Column::DatasetNames);

        auto datasetNames = datasetNamesIndex.data(Qt::EditRole).toStringList();
        
        datasetNames << datasetName;

        setData(index(0, Configurations::Column::Name), "Test");
        setData(index(0, Configurations::Column::DatasetNames), datasetNames);

        qDebug() << index(0, Configurations::Column::Name).data(Qt::EditRole).toString();
        //qDebug() << configurationsIndex.siblingAtColumn(Configurations::Column::DatasetNames).data(Qt::EditRole).toStringList();

        if (_configurations.getChildCount() == 1)
            selectRow(0);

        //qDebug() << data(index(0, Configurations::Column::DatasetNames), Qt::EditRole).toStringList();
    } else {
        auto datasetNames = hits.first().siblingAtRow(Channels::Child::Channel2).siblingAtColumn(Channel::Column::DatasetNames).data(Qt::EditRole).toStringList();

        datasetNames << datasetName;

        qDebug() << hits.first().siblingAtRow(Channels::Child::Channel2).siblingAtColumn(Channel::Column::DatasetNames).data(Qt::EditRole).toStringList();

        setData(hits.first().siblingAtRow(Channels::Child::Channel2).siblingAtColumn(Channel::Column::DatasetNames), datasetNames);
        setData(hits.first().siblingAtRow(Channels::Child::Channel3).siblingAtColumn(Channel::Column::DatasetNames), datasetNames);

        qDebug() << hits.first().siblingAtRow(Channels::Child::Channel2).siblingAtColumn(Channel::Column::DatasetNames).data(Qt::EditRole).toStringList();
    } 
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