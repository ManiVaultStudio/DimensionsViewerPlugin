#include "ConfigurationsModel.h"
#include "DimensionsViewerPlugin.h"
#include "Configurations.h"
#include "Configuration.h"
#include "Channels.h"
#include "Channel.h"

#include "PointData.h"

#include <QDebug>
#include <QMessageBox>

std::uint32_t ConfigurationsModel::noColumns = 0;

ConfigurationsModel::ConfigurationsModel(DimensionsViewerPlugin* dimensionsViewerPlugin) :
    QAbstractItemModel(static_cast<QObject*>(dimensionsViewerPlugin)),
	_dimensionsViewerPlugin(dimensionsViewerPlugin),
	_configurations(),
	_selectionModel(this),
    _datasetNames()
{
    const auto synchronizeProfiles = [this](const QModelIndex& channels) {
        const auto datasetChannel   = index(to_ul(Channels::Row::Dataset), 0, channels);
        const auto sourceProfile    = index(to_ul(Channel::Row::Profile), 0, datasetChannel);
        const auto profileIndices   = match(channels.siblingAtColumn(to_ul(TreeItem::Column::Type)), Qt::EditRole, "Profile", -1, Qt::MatchRecursive | Qt::MatchExactly);

        for (auto profile : profileIndices) {
            const auto channel              = profile.parent();
            const auto channelIndex         = channel.siblingAtColumn(to_ul(Channel::Column::Index)).data(Qt::EditRole).toInt();
            const auto isChannelLinked      = channel.siblingAtColumn(to_ul(Channel::Column::Linked)).data(Qt::EditRole).toBool();
            const auto isProfileAggregate   = profile.siblingAtColumn(to_ul(Profile::Column::IsAggregate)).data(Qt::EditRole).toBool();

            if (channelIndex >= 1 && isChannelLinked && !isProfileAggregate) {
                setData(profile.siblingAtColumn(to_ul(Profile::Column::ProfileTypes)), sourceProfile.siblingAtColumn(to_ul(Profile::Column::ProfileTypes)).data(Qt::EditRole), Qt::EditRole);
                setData(profile.siblingAtColumn(to_ul(Profile::Column::ProfileType)), sourceProfile.siblingAtColumn(to_ul(Profile::Column::ProfileType)).data(Qt::EditRole), Qt::EditRole);
                setData(profile.siblingAtColumn(to_ul(Profile::Column::RangeTypes)), sourceProfile.siblingAtColumn(to_ul(Profile::Column::RangeTypes)).data(Qt::EditRole), Qt::EditRole);
                setData(profile.siblingAtColumn(to_ul(Profile::Column::RangeType)), sourceProfile.siblingAtColumn(to_ul(Profile::Column::RangeType)).data(Qt::EditRole), Qt::EditRole);
            }
        }
    };

    const auto synchronizeStyling = [this](const QModelIndex& channels) {
        //const auto datasetChannel   = index(to_ul(Channels::Row::Dataset), 0, channels);
        //const auto sourceStyling    = index(to_ul(Channel::Row::Styling), 0, datasetChannel);
        //const auto stylingIndices   = match(channels.siblingAtColumn(to_ul(TreeItem::Column::Type)), Qt::EditRole, "Styling", -1, Qt::MatchRecursive | Qt::MatchExactly);

        //for (auto styling : stylingIndices) {
        //    const auto channel              = styling.parent();
        //    const auto channelIndex         = channel.siblingAtColumn(to_ul(Channel::Column::Index)).data(Qt::EditRole).toInt();
        //    const auto isChannelLinked      = channel.siblingAtColumn(to_ul(Channel::Column::Linked)).data(Qt::EditRole).toBool();
        //    const auto isProfileAggregate   = styling.siblingAtColumn(to_ul(Profile::Column::IsAggregate)).data(Qt::EditRole).toBool();

        //    if (channelIndex >= 1 && isChannelLinked && !isProfileAggregate) {
        //        setData(styling.siblingAtColumn(to_ul(Profile::Column::ProfileTypes)), sourceStyling.siblingAtColumn(to_ul(Profile::Column::ProfileTypes)).data(Qt::EditRole), Qt::EditRole);
        //        setData(styling.siblingAtColumn(to_ul(Profile::Column::ProfileType)), sourceStyling.siblingAtColumn(to_ul(Profile::Column::ProfileType)).data(Qt::EditRole), Qt::EditRole);
        //        setData(styling.siblingAtColumn(to_ul(Profile::Column::RangeTypes)), sourceStyling.siblingAtColumn(to_ul(Profile::Column::RangeTypes)).data(Qt::EditRole), Qt::EditRole);
        //        setData(styling.siblingAtColumn(to_ul(Profile::Column::RangeType)), sourceStyling.siblingAtColumn(to_ul(Profile::Column::RangeType)).data(Qt::EditRole), Qt::EditRole);
        //    }
        //}
    };

    QObject::connect(this, &QAbstractItemModel::dataChanged, [this, synchronizeProfiles, synchronizeStyling](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles = QVector<int>()) {
        const auto type = topLeft.siblingAtColumn(to_ul(TreeItem::Column::Type)).data(Qt::EditRole).toString();

        if (type == "Channel") {
            const auto column   = static_cast<Channel::Column>(topLeft.column());
            const auto channel  = topLeft.siblingAtColumn(to_ul(TreeItem::Column::Type));

            if (topLeft.column() == to_ul(TreeItem::Column::Enabled))
            {
                qDebug() << "Enabled";

                const auto channelIndex = topLeft.siblingAtColumn(0);

                emit dataChanged(channelIndex.siblingAtColumn(to_ul(Channel::Column::_Start)), channelIndex.siblingAtColumn(to_ul(Channel::Column::_End)));

                const auto profileIndex = index(to_ul(Channel::Row::Profile), 0, channelIndex);

                emit dataChanged(profileIndex.siblingAtColumn(to_ul(Profile::Column::_Start)), profileIndex.siblingAtColumn(to_ul(Profile::Column::_End)));

                
            }

            switch (column)
            {
                case Channel::Column::Linked:
                {
                    synchronizeProfiles(channel.parent());
                    synchronizeStyling(channel.parent());

                    break;
                }

                default:
                    break;
            }
        }

        if (type == "Profile") {
            const auto column = static_cast<Profile::Column>(topLeft.column());
            
            switch (column)
            {
                case Profile::Column::ProfileTypes:
                case Profile::Column::ProfileType:
                case Profile::Column::RangeTypes:
                case Profile::Column::RangeType:
                {
                    const auto channel = topLeft.siblingAtColumn(to_ul(TreeItem::Column::Type)).parent();

                    if (channel.siblingAtColumn(to_ul(Channel::Column::Index)).data(Qt::EditRole).toInt() == 0)
                        synchronizeProfiles(channel.parent());

                    break;
                }
                default:
                    break;
            }
        }
    });
}

int ConfigurationsModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    TreeItem *parentItem;

    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = const_cast<Configurations*>(&_configurations);
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->getChildCount();
}

int ConfigurationsModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    return noColumns;
}

QVariant ConfigurationsModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
    if (!index.isValid())
        return QVariant();

    /*
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();
    */

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

    const auto selectedConfiguration = getSelectedConfiguration();

    if (selectedConfiguration != nullptr) {
        emit configurationChanged(selectedConfiguration);
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
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return TreeItem::getColumnTypeName(static_cast<TreeItem::Column>(section));

    return QVariant();
}

QModelIndex ConfigurationsModel::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem* parentItem = nullptr;

    if (!parent.isValid())
        parentItem = const_cast<Configurations*>(&_configurations);
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

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

        Channels::Rows channels;

        channels << Channels::Row::Subset1;
        channels << Channels::Row::Subset2;
        channels << Channels::Row::Differential;

        for (auto channel : channels) {
            const auto channelIndex         = index(to_ul(channel), 0, channelsIndex);
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

TreeItem* ConfigurationsModel::getItem(const QModelIndex& index) const
{
    if (index.isValid()) {
        auto treeItem = static_cast<TreeItem*>(index.internalPointer());

        if (treeItem)
            return treeItem;
    }

    return static_cast<TreeItem*>(const_cast<Configurations*>(&_configurations));
}