#include "GlobalSettings.h"
#include "Configuration.h"

#include <QDebug>

GlobalSettings::GlobalSettings(ModelItem* parent) :
    ModelItem(parent),
    _enabled(true),
    _profile(Profile::ProfileType::Mean)
{
}

Qt::ItemFlags GlobalSettings::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = Qt::ItemIsSelectable;

    const auto column = static_cast<Column>(index.column());

    switch (column)
    {
        case Column::Title:
            break;

        case Column::Enabled: {
            flags |= Qt::ItemIsEnabled;
            break;
        }

        case Column::ProfileTypes:
            break;

        case Column::ProfileType: {
            flags |= Qt::ItemIsEnabled;
            break;
        }

        case Column::RangeTypes:
            break;

        case Column::RangeType: {
            if (!_profile.getRangeTypes().isEmpty())
                flags |= Qt::ItemIsEnabled;

            break;
        }

        default:
            break;
    }

    return flags;
}

QVariant GlobalSettings::getData(const QModelIndex& index, const int& role) const
{
    const auto column = static_cast<Column>(index.column());

    switch (column)
    {
        case Column::Title:
            return "Global settings";

        case Column::Enabled:
            return getEnabled(role);

        case Column::ProfileTypes:
            return getProfileTypes(role);

        case Column::ProfileType:
            return getProfileType(role);

        case Column::RangeTypes:
            return getRangeTypes(role);

        case Column::RangeType:
            return getRangeType(role);

        default:
            break;
    }

    return QVariant();
}

QModelIndexList GlobalSettings::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    QModelIndexList affectedIndices{ index };

    _modelIndex = index;

    const auto column = static_cast<Column>(index.column());

    switch (column)
    {
        case Column::Title:
            break;

        case Column::Enabled:
            affectedIndices << setEnabled(value.toBool());
            break;

        case Column::ProfileTypes:
            break;

        case Column::ProfileType:
            affectedIndices << setProfileType(Profile::getProfileTypeEnum(value.toString()));
            break;

        case Column::RangeTypes:
            break;

        case Column::RangeType:
            affectedIndices << setRangeType(Profile::getRangeTypeEnum(value.toString()));

        default:
            break;
    }

    return affectedIndices;
}

ModelItem* GlobalSettings::getChild(const int& index) const
{
    return nullptr;
}

int GlobalSettings::getChildCount() const
{
    return 0;
}

int GlobalSettings::getChildIndex(ModelItem* child) const
{
    return 0;
}

QVariant GlobalSettings::getEnabled(const std::int32_t& role) const
{
    const auto enabled = _enabled;
    const auto enabledString = enabled ? "on" : "off";

    switch (role)
    {
        case Qt::DisplayRole:
            return enabledString;

        case Qt::EditRole:
            return enabled;

        default:
            return QVariant();
    }

    return QVariant();
}

QModelIndexList GlobalSettings::setEnabled(const bool& enabled)
{
    QModelIndexList affectedIndices;

    if (enabled == _enabled)
        return affectedIndices;

    _enabled = enabled;

    if (_enabled) {
        for (int channelIndex = 0; channelIndex < Configuration::noChannels; channelIndex++) {
            //affectedColumns << setChannelProfileType(channelIndex, _globalProfile.getProfileType());
            //affectedColumns << setChannelRangeType(channelIndex, _globalProfile.getRangeType());
        }
    }

    //affectedColumns << Column::ShowDimensionNames;

    return affectedIndices;
}

QVariant GlobalSettings::getProfileTypes(const std::int32_t& role) const
{
    const auto profileTypes         = _profile.getProfileTypeNames();
    const auto profileTypesString   = profileTypes.join(", ");

    switch (role)
    {
        case Qt::DisplayRole:
            return profileTypesString;

        case Qt::EditRole:
            return profileTypes;

        default:
            return QVariant();
    }
}

QVariant GlobalSettings::getProfileType(const std::int32_t& role) const
{
    const auto profileTypeString = Profile::getProfileTypeName(_profile.getProfileType());

    switch (role)
    {
        case Qt::DisplayRole:
            return profileTypeString;

        case Qt::EditRole:
            return static_cast<int>(_profile.getProfileType());

        default:
            return QVariant();
    }
}

QModelIndexList GlobalSettings::setProfileType(const Profile::ProfileType& globalProfileType)
{
    QModelIndexList affectedIndices;

    //if (globalProfileType == _profile.getProfileType())
    //    return affectedIndices;

    _profile.setProfileType(globalProfileType);

    affectedIndices << _modelIndex.siblingAtColumn(static_cast<int>(Column::RangeTypes));
    affectedIndices << _modelIndex.siblingAtColumn(static_cast<int>(Column::RangeType));

    /*if (_enabled) {
        for (int channelIndex = 0; channelIndex < Configuration::noChannels; channelIndex++) {
            affectedColumns << setChannelProfileType(channelIndex, _globalProfile.getProfileType());
        }
    }*/

    return affectedIndices;
}

QVariant GlobalSettings::getRangeTypes(const std::int32_t& role) const
{
    const auto rangeTypes       = _profile.getRangeTypeNames();
    const auto rangeTypesString = rangeTypes.join(", ");

    switch (role)
    {
        case Qt::DisplayRole:
            return rangeTypesString;

        case Qt::EditRole:
            return rangeTypes;

        default:
            return QVariant();
    }
}

QVariant GlobalSettings::getRangeType(const std::int32_t& role) const
{
    const auto rangeTypeString = Profile::getRangeTypeName(_profile.getRangeType());

    switch (role)
    {
        case Qt::DisplayRole:
            return rangeTypeString;

        case Qt::EditRole:
            return static_cast<int>(_profile.getRangeType());

        default:
            return QVariant();
    }
}

QModelIndexList GlobalSettings::setRangeType(const Profile::RangeType& globalRangeType)
{
    QModelIndexList affectedIndices;

    //if (globalRangeType == _profile.getRangeType())
    //    return affectedIndices;

    _profile.setRangeType(globalRangeType);

    /*if (_enabled) {
        for (int channelIndex = 0; channelIndex < Configuration::noChannels; channelIndex++) {
            affectedColumns << setChannelRangeType(channelIndex, _profile.getRangeType());
        }
    }*/

    return affectedIndices;
}