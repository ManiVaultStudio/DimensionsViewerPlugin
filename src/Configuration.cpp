#include "Configuration.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

std::int32_t Configuration::maxNoDimensions = 100;
std::int32_t Configuration::noConfigurations = 0;

const QMap<QString, Configuration::Column> Configuration::columns = {
    { "Name", Configuration::Column::Name },
    { "Index", Configuration::Column::Index },
    { "Dataset name", Configuration::Column::DatasetName },
    { "Data name", Configuration::Column::DataName },
    { "Subsets", Configuration::Column::Subsets },
    { "Selection stamp", Configuration::Column::SelectionStamp }
};

const QMap<Configuration::Column, std::function<QVariant(Configuration* configuration, const QModelIndex& index)>> Configuration::getEditRoles = {
    { Configuration::Column::Name, [](Configuration* configuration, const QModelIndex& index) {
        return configuration->_name;
    }},
    { Configuration::Column::Index, [](Configuration* configuration, const QModelIndex& index) {
        return configuration->_index;
    }},
    { Configuration::Column::DatasetName, [](Configuration* configuration, const QModelIndex& index) {
        return configuration->_datasetName;
    }},
    { Configuration::Column::DataName, [](Configuration* configuration, const QModelIndex& index) {
        return configuration->_dataName;
    }},
    { Configuration::Column::Subsets, [](Configuration* configuration, const QModelIndex& index) {
        return configuration->_subsets;
    }}
};

const QMap<Configuration::Column, std::function<QVariant(Configuration* configuration, const QModelIndex& index)>> Configuration::getDisplayRoles = {
    { Configuration::Column::Name, [](Configuration* configuration, const QModelIndex& index) {
        return getEditRoles[Configuration::Column::Name](configuration, index);
    }},
    { Configuration::Column::Index, [](Configuration* configuration, const QModelIndex& index) {
        return QString::number(getEditRoles[Configuration::Column::Index](configuration, index).toInt());
    }},
    { Configuration::Column::DatasetName, [](Configuration* configuration, const QModelIndex& index) {
        return getEditRoles[Configuration::Column::DatasetName](configuration, index);
    }},
    { Configuration::Column::DataName, [](Configuration* configuration, const QModelIndex& index) {
        return getEditRoles[Configuration::Column::DataName](configuration, index);
    }},
    { Configuration::Column::Subsets, [](Configuration* configuration, const QModelIndex& index) {
        return getEditRoles[Configuration::Column::Subsets](configuration, index).toStringList().join(", ");
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

        QModelIndexList affectedIndices;

        for (int channelIndex = 0; channelIndex < 3; channelIndex++) {
            affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::Enabled);
            affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::Color);
            affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::Opacity);
            affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::ProfileTypes);
            affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::ProfileType);
            affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::RangeTypes);
            affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::RangeType);
            affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::Settings);
        }

        return affectedIndices;
    }}
};

Configuration::Configuration(ModelItem* parent, const QString& datasetName, const QString& dataName) :
    ModelItem("Configuration", parent),
	_index(noConfigurations),
    _datasetName(datasetName),
    _dataName(dataName),
	_channels(this, datasetName, dataName),
	_global(this),
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

int Configuration::columnCount() const 
{
    return ModelItem::maxNoColumns;
}

Qt::ItemFlags Configuration::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    const auto column = static_cast<Column>(index.column());

    switch (column)
    {
        default:
            break;
    }

    return flags;
}

QVariant Configuration::getData(const QModelIndex& index, const int& role) const
{
    const auto column = index.column();

    switch (role)
    {
        case Qt::EditRole:
        {
            if (getEditRoles.contains(static_cast<Column>(column)))
                return getEditRoles[static_cast<Column>(column)](const_cast<Configuration*>(this), index);

            break;
        }

        case Qt::DisplayRole:
        {
            if (getDisplayRoles.contains(static_cast<Column>(column)))
                return getDisplayRoles[static_cast<Column>(column)](const_cast<Configuration*>(this), index);

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

        case Child::Global:
            return const_cast<Global*>(&_global);

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

    if (dynamic_cast<Global*>(child))
        return static_cast<int>(Child::Global);

    return 0;
}

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