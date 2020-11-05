#include "GlobalSettings.h"
#include "Configuration.h"
#include "Channel.h"

#include <QDebug>

const QMap<GlobalSettings::Column, std::function<QVariant(GlobalSettings* globalSettings)>> GlobalSettings::getEditRoles = {
    { GlobalSettings::Column::Enabled, [](GlobalSettings* globalSettings) { return globalSettings->_enabled; }},
    { GlobalSettings::Column::ProfileTypes, [](GlobalSettings* globalSettings) { return globalSettings->_profile.getProfileTypeNames(); }},
    { GlobalSettings::Column::ProfileType, [](GlobalSettings* globalSettings) { return static_cast<int>(globalSettings->_profile.getProfileType()); }},
    { GlobalSettings::Column::RangeTypes, [](GlobalSettings* globalSettings) { return globalSettings->_profile.getRangeTypeNames(); }},
    { GlobalSettings::Column::RangeType, [](GlobalSettings* globalSettings) { return static_cast<int>(globalSettings->_profile.getRangeType()); }},
};

const QMap<GlobalSettings::Column, std::function<QVariant(GlobalSettings* globalSettings)>> GlobalSettings::getDisplayRoles = {
    { GlobalSettings::Column::Enabled, [](GlobalSettings* globalSettings) { return globalSettings->_enabled ? "on" : "off"; }},
    { GlobalSettings::Column::ProfileTypes, [](GlobalSettings* globalSettings) { return globalSettings->_profile.getProfileTypeNames().join(","); }},
    { GlobalSettings::Column::ProfileType, [](GlobalSettings* globalSettings) { return Profile::getProfileTypeName(globalSettings->_profile.getProfileType()); }},
    { GlobalSettings::Column::RangeTypes, [](GlobalSettings* globalSettings) { return globalSettings->_profile.getRangeTypeNames().join(","); }},
    { GlobalSettings::Column::RangeType, [](GlobalSettings* globalSettings) { return Profile::getRangeTypeName(globalSettings->_profile.getRangeType()); }},
};

const QMap<GlobalSettings::Column, std::function<QModelIndexList(GlobalSettings* globalSettings, const QVariant& value, const QModelIndex& index)>> GlobalSettings::setEditRoles = {
    { GlobalSettings::Column::Enabled, [](GlobalSettings* globalSettings, const QVariant& value, const QModelIndex& index) {
        globalSettings->_enabled = value.toBool();
        
        QModelIndexList affectedIndices;

        const auto channelsChild = static_cast<int>(Configuration::Child::Channels);

        for (int c = 0; c < 3; c++) {
            affectedIndices << index.parent().child(channelsChild, 0).child(c, Channel::Column::ProfileTypes);
            affectedIndices << index.parent().child(channelsChild, 0).child(c, Channel::Column::ProfileType);
            affectedIndices << index.parent().child(channelsChild, 0).child(c, Channel::Column::RangeTypes);
            affectedIndices << index.parent().child(channelsChild, 0).child(c, Channel::Column::RangeType);
        }

        return affectedIndices;
    }},
    { GlobalSettings::Column::ProfileType, [](GlobalSettings* globalSettings, const QVariant& value, const QModelIndex& index) {
        globalSettings->_profile.setProfileType(Profile::getProfileTypeEnum(value.toString()));

        QModelIndexList affectedIndices;

        affectedIndices << index.siblingAtColumn(GlobalSettings::Column::RangeTypes);
        affectedIndices << index.siblingAtColumn(GlobalSettings::Column::RangeType);

        return affectedIndices;
    }},
    { GlobalSettings::Column::RangeType, [](GlobalSettings* globalSettings, const QVariant& value, const QModelIndex& index) {
        globalSettings->_profile.setRangeType(Profile::getRangeTypeEnum(value.toString()));
        return QModelIndexList();
    }}
};

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
    return getData(index.column(), role);
}

QVariant GlobalSettings::getData(const int& column, const int& role) const
{
    switch (role)
    {
        case Qt::EditRole:
        {
            if (getEditRoles.contains(static_cast<Column>(column)))
                return getEditRoles[static_cast<Column>(column)](const_cast<GlobalSettings*>(this));

            break;
        }

        case Qt::DisplayRole:
        {
            if (getDisplayRoles.contains(static_cast<Column>(column)))
                return getDisplayRoles[static_cast<Column>(column)](const_cast<GlobalSettings*>(this));

            break;
        }

        default:
            break;
    }

    return QVariant();
}

QModelIndexList GlobalSettings::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    const auto column = static_cast<Column>(index.column());

    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole:
        {
            if (setEditRoles.contains(column))
                affectedIndices << setEditRoles[column](const_cast<GlobalSettings*>(this), value, index);

            break;
        }

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