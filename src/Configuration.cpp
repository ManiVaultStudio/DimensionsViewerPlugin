#include "Configuration.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

std::int32_t Configuration::maxNoDimensions = 100;
std::int32_t Configuration::noConfigurations = 0;

const QMap<QString, Configuration::Column> Configuration::columns = {
    { "Index", Configuration::Column::Index },
    { "Dataset name", Configuration::Column::DatasetName },
    { "Subsets", Configuration::Column::Subsets },
    { "Selection stamp", Configuration::Column::SelectionStamp }
};

const QMap<Configuration::Column, std::function<QVariant(Configuration* configuration)>> Configuration::getEditRoles = {
    { Configuration::Column::Index, [](Configuration* configuration) {
        return configuration->_index;
    }},
    { Configuration::Column::DatasetName, [](Configuration* configuration) {
        return configuration->_datasetName;
    }},
    { Configuration::Column::Subsets, [](Configuration* configuration) {
        return configuration->_subsets;
    }}
};

const QMap<Configuration::Column, std::function<QVariant(Configuration* configuration)>> Configuration::getDisplayRoles = {
    { Configuration::Column::Index, [](Configuration* configuration) {
        return QString::number(configuration->_index);
    }},
    { Configuration::Column::DatasetName, [](Configuration* configuration) {
        return configuration->_datasetName;
    }},
    { Configuration::Column::Subsets, [](Configuration* configuration) {
        return configuration->_subsets.join(", ");
    }}
};

const QMap<Configuration::Column, std::function<QModelIndexList(Configuration* configuration, const QVariant& value, const QModelIndex& index)>> Configuration::setEditRoles = {
    { Configuration::Column::Index, [](Configuration* configuration, const QVariant& value, const QModelIndex& index) {
        configuration->_index = value.toInt();
        return QModelIndexList();
    }},
    { Configuration::Column::DatasetName, [](Configuration* configuration, const QVariant& value, const QModelIndex& index) {
        configuration->_datasetName = value.toString();
        return QModelIndexList();
    }},
    { Configuration::Column::Subsets, [](Configuration* configuration, const QVariant& value, const QModelIndex& index) {
        configuration->_subsets = value.toStringList();
        return QModelIndexList();
    }}
};

Configuration::Configuration(ModelItem* parent, const QString& datasetName, const QString& dataName) :
    ModelItem(parent),
	_index(noConfigurations),
    _datasetName(datasetName),
	_channels(this, datasetName, dataName),
	_globalSettings(this),
	_subsets(),
    _showDifferentialProfile(false),
    _profileDatasetNames(),
    _profileDatasetName(),
    _showDimensionNames(false),
    _spec()
{
    noConfigurations++;

    _spec["modified"] = 0;
}

Qt::ItemFlags Configuration::getFlags(const QModelIndex& index) const
{
	Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;

    const auto column = static_cast<Column>(index.column());

	/*

	if (index.column() >= Column::ChannelOpacityStart && index.column() < Column::ChannelOpacityEnd) {
		const auto channelIndex = index.column() - Column::ChannelOpacityStart;

		if (channelIndex == 0) {
			flags |= Qt::ItemIsEnabled;
		}
		else {
			if (_channels[channelIndex]->isEnabled() && _subsets.size() >= channelIndex)
				flags |= Qt::ItemIsEnabled;
		}
	}

    if (column == Column::ShowDimensionNames) {
        if (getNoDisplayChannels() > 0)
            flags |= Qt::ItemIsEnabled;
    }

    if (column == Column::GlobalSettings) {
        if (getNoDisplayChannels() > 0)
            flags |= Qt::ItemIsEnabled;
    }

    if (column == Column::GlobalProfileType) {
        if (_globalSettings && !_globalProfile.getProfileTypeNames().isEmpty())
            flags |= Qt::ItemIsEnabled;
    }

    if (column == Column::GlobalRangeType) {
        if (_globalSettings && !_globalProfile.getRangeTypeNames().isEmpty())
            flags |= Qt::ItemIsEnabled;
    }
    
    if (column == Column::ShowDifferentialProfile) {
        if (canShowDifferentialProfile())
            flags |= Qt::ItemIsEnabled;
    }

    if (column == Column::Profile1DatasetName) {
        if (canShowDifferentialProfile() && _showDifferentialProfile && getNoChannelsEnabled() > 2)
            flags |= Qt::ItemIsEnabled;
    }

    if (column == Column::Profile2DatasetName) {
        if (canShowDifferentialProfile() && _showDifferentialProfile && getNoChannelsEnabled() > 2)
            flags |= Qt::ItemIsEnabled;
    }
    */

	return flags;
}

QVariant Configuration::getData(const QModelIndex& index, const int& role) const
{
    return getData(index.column(), role);
}

QVariant Configuration::getData(const int& column, const int& role) const
{
    switch (role)
    {
        case Qt::EditRole:
        {
            if (getEditRoles.contains(static_cast<Column>(column)))
                return getEditRoles[static_cast<Column>(column)](const_cast<Configuration*>(this));

            break;
        }

        case Qt::DisplayRole:
        {
            if (getDisplayRoles.contains(static_cast<Column>(column)))
                return getDisplayRoles[static_cast<Column>(column)](const_cast<Configuration*>(this));

            break;
        }

        default:
            break;
    }

    return QVariant();
}

QModelIndexList Configuration::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    const auto column = static_cast<Column>(index.column());

    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole:
        {
            if (setEditRoles.contains(column))
                affectedIndices << setEditRoles[column](const_cast<Configuration*>(this), value, index);

            break;
        }

        default:
            break;
    }

    return affectedIndices;
}

ModelItem* Configuration::getChild(const int& index) const
{
    switch (static_cast<Child>(index))
    {
        case Child::Channels:
            return const_cast<Channels*>(&_channels);

        case Child::GlobalSettings:
            return const_cast<GlobalSettings*>(&_globalSettings);

        case Child::DifferentialProfileSettings:
            return nullptr;

        case Child::MiscellaneousSettings:
            return nullptr;

        default:
            break;
    }

    return nullptr;
}

int Configuration::getChildCount() const
{
    return static_cast<int>(Child::End);
}

int Configuration::getChildIndex(ModelItem* child) const
{
    if (dynamic_cast<Channels*>(child))
        return static_cast<int>(Child::Channels);

    if (dynamic_cast<GlobalSettings*>(child))
        return static_cast<int>(Child::GlobalSettings);

    return 0;
}





//


//QVariant Configuration::getShowDimensionNames(const std::int32_t& role) const
//{
//    const auto showDimensionNames       = _showDimensionNames;
//    const auto showDimensionNamesString = showDimensionNames ? "on" : "off";
//
//    switch (role)
//    {
//        case Qt::DisplayRole:
//            return showDimensionNamesString;
//
//        case Qt::EditRole:
//            return showDimensionNames;
//
//        case Qt::ToolTipRole:
//            return getTooltip(Column::ShowDimensionNames, showDimensionNamesString);
//
//        default:
//            return QVariant();
//    }
//
//    return QVariant();
//}
//
//Configuration::AffectedColumns Configuration::setShowDimensionNames(const bool& showDimensionNames)
//{
//    AffectedColumns affectedColumns{ Column::ShowDimensionNames };
//
//    if (showDimensionNames == _showDimensionNames)
//        return affectedColumns;
//
//    _showDimensionNames = showDimensionNames;
//    _spec["modified"] = _spec["modified"].toInt() + 1;
//
//    return affectedColumns;
//}

//QVariant Configuration::getShowDifferentialProfile(const std::int32_t& role) const
//{
//    const auto showDifferentialProfile          = _showDifferentialProfile;
//    const auto showDifferentialProfileString    = showDifferentialProfile ? "on" : "off";
//
//    switch (role)
//    {
//        case Qt::DisplayRole:
//            return showDifferentialProfileString;
//
//        case Qt::EditRole:
//            return showDifferentialProfile;
//
//        case Qt::ToolTipRole:
//            return getTooltip(Column::ShowDifferentialProfile, showDifferentialProfileString);
//
//        default:
//            return QVariant();
//    }
//
//    return QVariant();
//}
//
//Configuration::AffectedColumns Configuration::setShowDifferentialProfile(const bool& showDifferentialProfile)
//{
//    AffectedColumns affectedColumns{ Column::ShowDifferentialProfile };
//
//    if (showDifferentialProfile == _showDifferentialProfile)
//        return affectedColumns;
//
//    _showDifferentialProfile = showDifferentialProfile;
//
//    affectedColumns << updateDifferentialProfile();
//
//    return affectedColumns;
//}
//
//QVariant Configuration::getProfileDatasetNames(const std::int32_t& profileIndex, const std::int32_t& role) const
//{
//    try
//    {
//        const auto profileDatasetNames          = _profileDatasetNames[profileIndex];
//        const auto profileDatasetNamesString    = profileDatasetNames.join(", ");
//
//        switch (role)
//        {
//            case Qt::DisplayRole:
//                return profileDatasetNamesString;
//
//            case Qt::EditRole:
//                return profileDatasetNames;
//
//            case Qt::ToolTipRole:
//                return getTooltip(Column::Profile1DatasetNames + profileIndex, profileDatasetNamesString);
//
//            default:
//                return QVariant();
//        }
//    }
//    catch (std::exception exception)
//    {
//        qDebug() << exception.what();
//    }
//
//    return QVariant();
//}
//
//Configuration::AffectedColumns Configuration::setProfileDatasetNames(const std::int32_t& profileIndex, const QStringList& datasetNames)
//{
//    AffectedColumns affectedColumns{ Column::Profile1DatasetNames + profileIndex };
//
//    try
//    {
//        if (datasetNames == _profileDatasetNames[profileIndex])
//            return affectedColumns;
//
//        _profileDatasetNames[profileIndex] = datasetNames;
//    }
//    catch (std::exception exception)
//    {
//        qDebug() << exception.what();
//    }
//
//    return affectedColumns;
//}
//
//QVariant Configuration::getProfileDatasetName(const std::int32_t& profileIndex, const std::int32_t& role) const
//{
//    try
//    {
//        const auto profileDatasetName = _profileDatasetName[profileIndex];
//
//        switch (role)
//        {
//            case Qt::DisplayRole:
//                return profileDatasetName;
//
//            case Qt::EditRole:
//                return profileDatasetName;
//
//            case Qt::ToolTipRole:
//                return getTooltip(Column::Profile1DatasetName + profileIndex, profileDatasetName);
//
//            default:
//                return QVariant();
//        }
//    }
//    catch (std::exception exception)
//    {
//        qDebug() << exception.what();
//    }
//
//    return QVariant();
//}
//
//Configuration::AffectedColumns Configuration::setProfileDatasetName(const std::int32_t& profileIndex, const QString& datasetName)
//{
//    AffectedColumns affectedColumns{Column::Profile1DatasetName + profileIndex};
//
//    try
//    {
//        if (datasetName == _profileDatasetName[profileIndex])
//            return affectedColumns;
//
//        _profileDatasetName[profileIndex] = datasetName;
//    }
//    catch (std::exception exception)
//    {
//        qDebug() << exception.what();
//    }
//
//    return affectedColumns;
//}

bool Configuration::hasDataset(const QString& datasetName) const
{
	return const_cast<Configuration*>(this)->_channels.getChannelByDatasetName(datasetName) != nullptr;
}

void Configuration::updateSpec()
{
    /*QVariantMap channels;

    for (auto channel : _channels) {
        if (!channel->isEnabled())
            continue;

        channels[channel->getInternalName()] = channel->getSpec();
    }

    _spec["channels"]               = channels;
    _spec["showDimensionNames"]     = _showDimensionNames;*/
}

QVariantMap Configuration::getSpec() const
{
	return _spec;
}

std::int32_t Configuration::getModified() const
{
    return _spec["modified"].toInt();
}

bool Configuration::canShowDifferentialProfile() const
{
    return _channels.getNoDisplayChannels() >= 2;
}

QString Configuration::getTooltip(const std::int32_t& column, const QString& description) const
{
    const auto columnName = getColumnName(static_cast<Configuration::Column>(column));

    if (description.isEmpty())
        return columnName;

    return QString("%1: %2").arg(columnName, description);
}

//Configuration::AffectedColumns Configuration::updateDifferentialProfile()
//{
//    AffectedColumns affectedColumns;
//
//    const auto resetDifferentialProfile = [this, &affectedColumns]() {
//        affectedColumns << setProfileDatasetName(0, "");
//        affectedColumns << setProfileDatasetName(1, "");
//
//        affectedColumns << setProfileDatasetNames(0, QStringList());
//        affectedColumns << setProfileDatasetNames(1, QStringList());
//    };
//
//    const auto getAllDatasetNames = [this]() -> QStringList {
//        QStringList allDatasetNames;
//
//        for (auto channel : _channels) {
//            if (channel->isEnabled())
//                allDatasetNames << channel->getDatasetName();
//        }
//
//        return allDatasetNames;
//    };
//
//    const auto getDatasetName = [this, &getAllDatasetNames](const std::uint32_t& profileIndex) -> QString {
//        const auto opposingProfileIndex = profileIndex == 0 ? 1 : 0;
//        const auto otherDatasetName = _profileDatasetName[opposingProfileIndex];
//        const auto profileDatasetName = _profileDatasetName[profileIndex];
//
//        auto datasetNames = getAllDatasetNames();
//
//        datasetNames.removeOne(otherDatasetName);
//
//        if (datasetNames.contains(profileDatasetName))
//            return profileDatasetName;
//
//        return datasetNames.first();
//    };
//
//    const auto getDatasetNames = [this, &getAllDatasetNames](const std::uint32_t& profileIndex) -> QStringList {
//        const auto opposingProfileIndex = profileIndex == 0 ? 1 : 0;
//        const auto otherDatasetName = _profileDatasetName[opposingProfileIndex];
//
//        auto datasetNames = getAllDatasetNames();
//
//        datasetNames.removeOne(otherDatasetName);
//
//        return datasetNames;
//    };
//
//    if (!canShowDifferentialProfile()) {
//        resetDifferentialProfile();
//    }
//    else {
//        if (_showDifferentialProfile) {
//            const auto channelsEnabled = getChannelsEnabled();
//            //const auto opposingProfileIndex = profileIndex == 0 ? 1 : 0;
//            //const auto opposingDatasetName  = getChannelDatasetName(opposingProfileIndex, Qt::EditRole).toString();
//
//            switch (channelsEnabled.size())
//            {
//                case 2: {
//                    affectedColumns << setProfileDatasetName(0, getChannelDatasetName(channelsEnabled[0], Qt::EditRole).toString());
//                    affectedColumns << setProfileDatasetName(1, getChannelDatasetName(channelsEnabled[1], Qt::EditRole).toString());
//                    break;
//                }
//
//                case 3: {
//                    if (_profileDatasetName[0] == "")
//                        affectedColumns << setProfileDatasetName(0, getChannelDatasetName(1, Qt::EditRole).toString());
//
//                    if (_profileDatasetName[1] == "")
//                        affectedColumns << setProfileDatasetName(1, getChannelDatasetName(2, Qt::EditRole).toString());
//
//                    break;
//                }
//
//                default:
//                    break;
//            }
//
//            affectedColumns << setProfileDatasetNames(0, getDatasetNames(0));
//            affectedColumns << setProfileDatasetNames(1, getDatasetNames(1));
//        }
//        else {
//            resetDifferentialProfile();
//        }
//    }
//
//    return affectedColumns;
//}