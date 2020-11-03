#include "GlobalSettings.h"
#include "Configuration.h"

#include <QDebug>

GlobalSettings::GlobalSettings(QObject* parent) :
    QObject(parent),
    _enabled(true),
    _profile(Profile::ProfileType::Mean)
{
}

Qt::ItemFlags GlobalSettings::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEditable;

    return flags;
}

QVariant GlobalSettings::getData(const QModelIndex& index, const int& role) const
{
    /*if (column == Column::GlobalSettings)
        return getGlobalSettings(role);

    if (column == Column::GlobalProfileTypes)
        return getGlobalProfileTypes(role);

    if (column == Column::GlobalProfileType)
        return getGlobalProfileType(role);

    if (column == Column::GlobalRangeTypes)
        return getGlobalRangeTypes(role);

    if (column == Column::GlobalRangeType)
        return getGlobalRangeType(role);*/

    return QVariant();
}

AffectedColumns GlobalSettings::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    AffectedColumns affectedColumns;

    return affectedColumns;
}

//if (index.column() == Column::GlobalSettings)
//affectedColumns << setGlobalSettings(value.toBool());
//
//if (index.column() == Column::GlobalProfileType)
//affectedColumns << setGlobalProfileType(Profile::getProfileTypeEnum(value.toString()));
//
//if (index.column() == Column::GlobalRangeType)
//affectedColumns << setGlobalRangeType(Profile::getRangeTypeEnum(value.toString()));




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

AffectedColumns GlobalSettings::setEnabled(const bool& enabled)
{
    AffectedColumns affectedColumns{ Column::Enabled };

    if (enabled == _enabled)
        return affectedColumns;

    _enabled = enabled;

    if (_enabled) {
        for (int channelIndex = 0; channelIndex < Configuration::noChannels; channelIndex++) {
            //affectedColumns << setChannelProfileType(channelIndex, _globalProfile.getProfileType());
            //affectedColumns << setChannelRangeType(channelIndex, _globalProfile.getRangeType());
        }
    }

    //affectedColumns << Column::ShowDimensionNames;

    return affectedColumns;
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

AffectedColumns GlobalSettings::setProfileType(const Profile::ProfileType& globalProfileType)
{
    AffectedColumns affectedColumns{ Column::ProfileType };

    if (globalProfileType == _profile.getProfileType())
        return affectedColumns;

    _profile.setProfileType(globalProfileType);

    affectedColumns << Column::RangeType;
    affectedColumns << Column::RangeTypes;

    /*if (_enabled) {
        for (int channelIndex = 0; channelIndex < Configuration::noChannels; channelIndex++) {
            affectedColumns << setChannelProfileType(channelIndex, _globalProfile.getProfileType());
        }
    }*/

    return affectedColumns;
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

AffectedColumns GlobalSettings::setRangeType(const Profile::RangeType& globalRangeType)
{
    AffectedColumns affectedColumns{ Column::RangeType };

    if (globalRangeType == _profile.getRangeType())
        return affectedColumns;

    _profile.setRangeType(globalRangeType);

    /*if (_enabled) {
        for (int channelIndex = 0; channelIndex < Configuration::noChannels; channelIndex++) {
            affectedColumns << setChannelRangeType(channelIndex, _profile.getRangeType());
        }
    }*/

    return affectedColumns;
}