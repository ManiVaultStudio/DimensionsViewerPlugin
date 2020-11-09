#include "Configuration.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

std::int32_t Configuration::maxNoDimensions = 100;
std::int32_t Configuration::noConfigurations = 0;

const QMap<QString, Configuration::Columns> Configuration::columns = {
    { "Name", Configuration::Columns::Name },
    { "Index", Configuration::Columns::Index },
    { "Dataset name", Configuration::Columns::DatasetName },
    { "Data name", Configuration::Columns::DataName },
    { "Subsets", Configuration::Columns::Subsets },
    { "Selection stamp", Configuration::Columns::SelectionStamp }
};

const QMap<Configuration::Columns, std::function<QVariant(Configuration* configuration)>> Configuration::getEditRoles = {
    { Configuration::Columns::Name, [](Configuration* configuration) {
        return configuration->_name;
    }},
    { Configuration::Columns::Index, [](Configuration* configuration) {
        return configuration->_index;
    }},
    { Configuration::Columns::DatasetName, [](Configuration* configuration) {
        return configuration->_datasetName;
    }},
    { Configuration::Columns::DataName, [](Configuration* configuration) {
        return configuration->_dataName;
    }},
    { Configuration::Columns::Subsets, [](Configuration* configuration) {
        return configuration->_subsets;
    }}
};

const QMap<Configuration::Columns, std::function<QVariant(Configuration* configuration)>> Configuration::getDisplayRoles = {
    { Configuration::Columns::Name, [](Configuration* configuration) {
        return getEditRoles[Configuration::Columns::Name](configuration);
    }},
    { Configuration::Columns::Index, [](Configuration* configuration) {
        return QString::number(getEditRoles[Configuration::Columns::Index](configuration).toInt());
    }},
    { Configuration::Columns::DatasetName, [](Configuration* configuration) {
        return getEditRoles[Configuration::Columns::DatasetName](configuration);
    }},
    { Configuration::Columns::DataName, [](Configuration* configuration) {
        return getEditRoles[Configuration::Columns::DataName](configuration);
    }},
    { Configuration::Columns::Subsets, [](Configuration* configuration) {
        return getEditRoles[Configuration::Columns::Subsets](configuration).toStringList().join(", ");
    }}
};

const QMap<Configuration::Columns, std::function<QModelIndexList(Configuration* configuration, const QModelIndex& index, const QVariant& value)>> Configuration::setEditRoles = {
    { Configuration::Columns::Index, [](Configuration* configuration, const QModelIndex& index, const QVariant& value) {
        configuration->_index = value.toInt();
        return QModelIndexList();
    }},
    { Configuration::Columns::DatasetName, [](Configuration* configuration, const QModelIndex& index, const QVariant& value) {
        configuration->_datasetName = value.toString();
        return QModelIndexList();
    }},
    { Configuration::Columns::Subsets, [](Configuration* configuration, const QModelIndex& index, const QVariant& value) {
        configuration->_subsets = value.toStringList();

        QModelIndexList affectedIndices;

        QVector<std::int32_t> channels;

        channels << Channels::Rows::Channel2;
        channels << Channels::Rows::Channel3;

        const auto channelsIndex = configuration->index(0, 0, index.siblingAtColumn(0));

        for (auto channel : channels) {
            for (int column = to_ul(Channel::Columns::Start); column <= to_ul(Channel::Columns::End); column++)
                affectedIndices << configuration->index(channel, 0, channelsIndex).siblingAtColumn(column);
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
	_differentialProfile(this),
	_subsets(),
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

    const auto column = static_cast<Columns>(index.column());

    switch (column)
    {
        case Columns::Name:
        case Columns::Index:
        case Columns::DatasetName:
        case Columns::DataName:
        case Columns::Subsets:
        case Columns::SelectionStamp:
        {
            flags |= Qt::ItemIsEnabled;

            break;
        }

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
            if (getEditRoles.contains(static_cast<Columns>(column)))
                return getEditRoles[static_cast<Columns>(column)](const_cast<Configuration*>(this));

            break;
        }

        case Qt::DisplayRole:
        {
            if (getDisplayRoles.contains(static_cast<Columns>(column)))
                return getDisplayRoles[static_cast<Columns>(column)](const_cast<Configuration*>(this));

            break;
        }

        default:
            break;
    }

    return QVariant();
}

QModelIndexList Configuration::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    const auto column = static_cast<Columns>(index.column());

    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole:
        {
            if (setEditRoles.contains(column))
                affectedIndices << setEditRoles[column](const_cast<Configuration*>(this), index, value);

            break;
        }

        default:
            break;
    }

    return affectedIndices;
}

ModelItem* Configuration::getChild(const int& index) const
{
    switch (static_cast<Rows>(index))
    {
        case Rows::Channels:
            return const_cast<Channels*>(&_channels);

        case Rows::Global:
            return const_cast<Global*>(&_global);

        case Rows::DifferentialProfile:
            return const_cast<DifferentialProfile*>(&_differentialProfile);

        case Rows::Miscellaneous:
            return nullptr;

        default:
            break;
    }

    return nullptr;
}

int Configuration::getChildCount() const
{
    return static_cast<int>(Rows::End);
}

int Configuration::getChildIndex(ModelItem* child) const
{
    if (dynamic_cast<Channels*>(child))
        return static_cast<int>(Rows::Channels);

    if (dynamic_cast<Global*>(child))
        return static_cast<int>(Rows::Global);

    if (dynamic_cast<DifferentialProfile*>(child))
        return static_cast<int>(Rows::DifferentialProfile);

    return 0;
}

const Channels* Configuration::getChannels() const
{
    return &_channels;
}

const Global* Configuration::getGlobal() const
{
    return &_global;
}

const DifferentialProfile* Configuration::getDifferentialProfile() const
{
    return &_differentialProfile;
}

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