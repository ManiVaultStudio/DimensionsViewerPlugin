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

    const auto column = static_cast<Column>(index.column());

    switch (column)
    {
        case Column::Name:
        case Column::Index:
        case Column::DatasetName:
        case Column::DataName:
        case Column::Subsets:
        case Column::SelectionStamp:
        {
            flags |= Qt::ItemIsEnabled;

            break;
        }

        default:
            break;
    }

    return flags;
}

QVariant Configuration::getData(const std::int32_t& column, const std::int32_t& role) const
{
    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(column))
            {
                case Configuration::Column::Name:
                    return _name;

                case Configuration::Column::Index:
                    return _index;

                case Configuration::Column::DatasetName:
                    return _datasetName;

                case Configuration::Column::DataName:
                    return _dataName;

                case Configuration::Column::Subsets:
                    return _subsets;

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole: {

            switch (static_cast<Column>(column))
            {
                case Configuration::Column::Name:
                    return getData(column, Qt::EditRole);

                case Configuration::Column::Index:
                    return QString::number(getData(column, Qt::EditRole).toInt());

                case Configuration::Column::DatasetName:
                    return getData(column, Qt::EditRole);

                case Configuration::Column::DataName:
                    return getData(column, Qt::EditRole);

                case Configuration::Column::Subsets:
                    return getData(column, Qt::EditRole).toStringList().join(", ");

                default:
                    break;
            }

            break;
        }

        default:
            break;
    }

    return QVariant();
}

ModelItem::AffectedColumns Configuration::setData(const std::int32_t& column, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    AffectedColumns affectedColunns{ column };

    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(column))
            {
                case Configuration::Column::Subsets:
                {
                    _subsets = value.toStringList();

                    /*QModelIndexList affectedIndices;

                    QVector<std::int32_t> channels;

                    channels << Channels::Row::Channel2;
                    channels << Channels::Row::Channel3;

                    const auto channelsIndex = configuration->index(0, 0, index.siblingAtColumn(0));

                    for (auto channel : channels) {
                        for (int column = to_ul(Channel::Column::Start); column <= to_ul(Channel::Column::End); column++)
                            affectedIndices << configuration->index(channel, 0, channelsIndex).siblingAtColumn(column);
                    }*/

                    break;
                }

                default:
                    break;
            }

            break;
        }

        default:
            break;
    }

    return affectedColunns;
}

ModelItem* Configuration::getChild(const int& index) const
{
    switch (static_cast<Row>(index))
    {
        case Row::Channels:
            return const_cast<Channels*>(&_channels);

        case Row::Global:
            return const_cast<Global*>(&_global);

        case Row::DifferentialProfile:
            return const_cast<DifferentialProfile*>(&_differentialProfile);

        case Row::Miscellaneous:
            return nullptr;

        default:
            break;
    }

    return nullptr;
}

int Configuration::getChildCount() const
{
    return static_cast<int>(Row::End);
}

int Configuration::getChildIndex(ModelItem* child) const
{
    if (dynamic_cast<Channels*>(child))
        return static_cast<int>(Row::Channels);

    if (dynamic_cast<Global*>(child))
        return static_cast<int>(Row::Global);

    if (dynamic_cast<DifferentialProfile*>(child))
        return static_cast<int>(Row::DifferentialProfile);

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