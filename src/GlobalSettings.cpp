#include "GlobalSettings.h"
#include "Configuration.h"
#include "Channel.h"
#include "DimensionsViewerPlugin.h"
#include "ConfigurationsModel.h"

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

        const auto channelsIndex = index.model()->index(0, 0, index.parent());

        for (int c = 0; c < 3; c++) {
            const auto channelIndex = index.model()->index(c, 0, channelsIndex);

            affectedIndices << channelIndex.siblingAtColumn(Channel::Column::ProfileTypes);
            affectedIndices << channelIndex.siblingAtColumn(Channel::Column::ProfileType);
            affectedIndices << channelIndex.siblingAtColumn(Channel::Column::RangeTypes);
            affectedIndices << channelIndex.siblingAtColumn(Channel::Column::RangeType);
        }

        return affectedIndices;
    }},
    { GlobalSettings::Column::ProfileType, [](GlobalSettings* globalSettings, const QVariant& value, const QModelIndex& index) {
        globalSettings->_profile.setProfileType(Profile::getProfileTypeEnum(value.toString()));

        QModelIndexList affectedIndices;

        affectedIndices << index.siblingAtColumn(GlobalSettings::Column::RangeTypes);
        affectedIndices << index.siblingAtColumn(GlobalSettings::Column::RangeType);

        if (globalSettings->_enabled) {
            const auto channelsIndex = index.model()->index(0, 0, index.parent());

            auto& configurationsModel = globalSettings->dimensionsViewerPlugin->getConfigurationsModel();
            
            for (int c = 0; c < 3; c++) {
                const auto channelIndex = index.model()->index(c, 0, channelsIndex);

                configurationsModel.setData(channelIndex.siblingAtColumn(Channel::Column::ProfileType), Profile::getProfileTypeName(globalSettings->_profile.getProfileType()), Qt::EditRole);
                configurationsModel.setData(channelIndex.siblingAtColumn(Channel::Column::RangeTypes), globalSettings->_profile.getRangeTypeNames(), Qt::EditRole);
                configurationsModel.setData(channelIndex.siblingAtColumn(Channel::Column::RangeType), Profile::getRangeTypeName(globalSettings->_profile.getRangeType()), Qt::EditRole);
            }
        }

        return affectedIndices;
    }},
    { GlobalSettings::Column::RangeType, [](GlobalSettings* globalSettings, const QVariant& value, const QModelIndex& index) {
        globalSettings->_profile.setRangeType(Profile::getRangeTypeEnum(value.toString()));
        
        QModelIndexList affectedIndices;

        if (globalSettings->_enabled) {
            const auto channelsChild = static_cast<int>(Configuration::Child::Channels);

            for (int c = 0; c < 3; c++)
                affectedIndices << index.parent().child(channelsChild, 0).child(c, Channel::Column::RangeType);
        }

        return affectedIndices;
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