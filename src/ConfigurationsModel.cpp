#include "ConfigurationsModel.h"
#include "DimensionsViewerPlugin.h"
#include "Configurations.h"
#include "Configuration.h"

#include <QDebug>
#include <QMessageBox>

ConfigurationsModel::ConfigurationsModel(DimensionsViewerPlugin* dimensionsViewerPlugin) :
    QAbstractItemModel(static_cast<QObject*>(dimensionsViewerPlugin)),
	_dimensionsViewerPlugin(dimensionsViewerPlugin),
	_root(),
	_selectionModel(this)
{
    tree::Item::setModel(this);
    
    _root.setModelIndex(QModelIndex());
}

int ConfigurationsModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    tree::Item *parentItem;

    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = const_cast<Root*>(&_root);
    else
        parentItem = static_cast<tree::Item*>(parent.internalPointer());

    return parentItem->getChildCount();
}

int ConfigurationsModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    return to_ul(tree::Item::Column::_Count);
}

QVariant ConfigurationsModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
    if (!index.isValid())
        return QVariant();

    /*
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();
    */

    const auto item = index.parent().isValid() ? getItem(index) : &_root;

    return getItem(index)->getData(index, role);
}

bool ConfigurationsModel::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
{
    if (index == QModelIndex())
        return false;

    auto item = getItem(index);

    if (item->setData(index, value, role)) {
        emit dataChanged(index, index);
        emit item->dataChanged(index);

        return true;
    }

    return false;
}

Qt::ItemFlags ConfigurationsModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return getItem(index)->getFlags(index);
}

QVariant ConfigurationsModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return tree::Item::getColumnTypeName(static_cast<tree::Item::Column>(section));

    return QVariant();
}

QModelIndex ConfigurationsModel::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    tree::Item* parentItem = nullptr;

    if (!parent.isValid())
        parentItem = const_cast<Root*>(&_root);
    else
        parentItem = static_cast<tree::Item*>(parent.internalPointer());

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

    if (parentItem == &_root || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->getChildIndex(), 0, parentItem);
}

void ConfigurationsModel::addDataset(const QString& datasetName)
{
    const auto dataName = _dimensionsViewerPlugin->getCore()->requestData<Points>(datasetName).getDataName();

    auto configurations = reinterpret_cast<Configurations*>(_root.getChild("Configurations"));
    auto configuration = configurations->getConfigurationByDataName(dataName);

    if (configuration == nullptr) {
        const auto noConfigurations = configurations->getChildCount();

        beginInsertRows(configurations->getModelIndex(), noConfigurations, noConfigurations);
        {
            configurations->add(datasetName, dataName);
        }
        endInsertRows();
        
        /*
        auto datasetNames = _root.getChild("DatasetNames")->getValue().toStringList();

        datasetNames << datasetName;

        _root.getChild("DatasetNames")->setValue(datasetNames);

        if (_root.getChildCount() == 1)
            selectRow(0);
        */
    } else {

        /*auto subset1DatasetNames = configuration->getChild("Channels/Subset1/DatasetNames")->getValue().toStringList();
        auto subset2DatasetNames = configuration->getChild("Channels/Subset2/DatasetNames")->getValue().toStringList();

        subset1DatasetNames << datasetName;
        subset2DatasetNames << datasetName;

        configuration->getChild("Channels/Subset1/DatasetNames")->setValue(subset1DatasetNames);
        configuration->getChild("Channels/Subset2/DatasetNames")->setValue(subset2DatasetNames);*/
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

        /*if (firstChannelIndex.siblingAtColumn(to_ul(ChannelItem::Column::NoDimensions)).data(Qt::EditRole).toInt() > Configuration::maxNoDimensions) {
            const auto datasetName = configurationIndex.siblingAtColumn(to_ul(Configuration::Column::DatasetName)).data(Qt::EditRole).toString();
            throw std::runtime_error(QString("%1 has more than %2 dimensions").arg(datasetName, QString::number(Configuration::maxNoDimensions)).toLatin1());
        }
        else {
            _selectionModel.select(configurationIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
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

void ConfigurationsModel::selectRow(const QString& datasetName)
{
    Q_ASSERT(!datasetName.isEmpty());

    const auto dataName = _dimensionsViewerPlugin->getCore()->requestData<Points>(datasetName).getDataName();
    const auto hits     = match(QModelIndex(), Qt::DisplayRole, dataName, -1, Qt::MatchExactly | Qt::MatchRecursive);

    if (!hits.isEmpty())
        selectRow(hits.first().parent().row());
}

tree::Item* ConfigurationsModel::getItem(const QModelIndex& modelIndex) const
{
    if (modelIndex.isValid()) {
        auto treeItem = static_cast<tree::Item*>(modelIndex.internalPointer());

        if (treeItem)
            return treeItem;
    }

    return static_cast<tree::Item*>(const_cast<Root*>(&_root));
}