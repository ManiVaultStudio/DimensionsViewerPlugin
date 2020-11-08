#include "Global.h"
#include "Configuration.h"
#include "Channel.h"
#include "DimensionsViewerPlugin.h"
#include "ConfigurationsModel.h"

#include <QDebug>

const QMap<QString, Global::Column> Global::columns = {
    { "Name", Global::Column::Name },
    { "Enabled", Global::Column::Enabled },
    { "Profile types", Global::Column::ProfileTypes },
    { "Profile type", Global::Column::ProfileType },
    { "Range types", Global::Column::RangeTypes },
    { "Range type", Global::Column::RangeType }
};

const QMap<Global::Column, std::function<QVariant(Global* global)>> Global::getEditRoles = {
    { Global::Column::Name, [](Global* global) {
        return global->_name;
    }},
    { Global::Column::Enabled, [](Global* global) {
        return global->_enabled;
    }},
    { Global::Column::ProfileTypes, [](Global* global) {
        return global->_profile.getProfileTypeNames();
    }},
    { Global::Column::ProfileType, [](Global* global) {
        return static_cast<int>(global->_profile.getProfileType());
    }},
    { Global::Column::RangeTypes, [](Global* global) {
        return global->_profile.getRangeTypeNames();
    }},
    { Global::Column::RangeType, [](Global* global) {
        return static_cast<int>(global->_profile.getRangeType());
    }}
};

const QMap<Global::Column, std::function<QVariant(Global* global)>> Global::getDisplayRoles = {
    { Global::Column::Name, [](Global* global) {
        return global->_name;
    }},
    { Global::Column::Enabled, [](Global* global) {
        return global->_enabled ? "on" : "off";
    }},
    { Global::Column::ProfileTypes, [](Global* global) {
        return global->_profile.getProfileTypeNames().join(",");
    }},
    { Global::Column::ProfileType, [](Global* global) {
        return Profile::getProfileTypeName(global->_profile.getProfileType());
    }},
    { Global::Column::RangeTypes, [](Global* global) {
        return global->_profile.getRangeTypeNames().join(",");
    }},
    { Global::Column::RangeType, [](Global* global) {
        return Profile::getRangeTypeName(global->_profile.getRangeType());
    }}
};

const QMap<Global::Column, std::function<QModelIndexList(Global* global, const QVariant& value, const QModelIndex& index)>> Global::setEditRoles = {
    { Global::Column::Enabled, [](Global* global, const QVariant& value, const QModelIndex& index) {
        global->_enabled = value.toBool();

        QModelIndexList affectedIndices;

        affectedIndices << index.siblingAtColumn(Global::Column::ProfileTypes);
        affectedIndices << index.siblingAtColumn(Global::Column::ProfileType);
        affectedIndices << index.siblingAtColumn(Global::Column::RangeTypes);
        affectedIndices << index.siblingAtColumn(Global::Column::RangeType);
        
        for (int channelIndex = 0; channelIndex < 3; channelIndex++) {
            affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::ProfileTypes);
            affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::ProfileType);
            affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::RangeTypes);
            affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::RangeType);
        }

        return affectedIndices;
    }},
    { Global::Column::ProfileType, [](Global* global, const QVariant& value, const QModelIndex& index) {
        global->_profile.setProfileType(Profile::getProfileTypeEnum(value.toString()));

        QModelIndexList affectedIndices;

        affectedIndices << index.siblingAtColumn(Global::Column::RangeTypes);
        affectedIndices << index.siblingAtColumn(Global::Column::RangeType);

        if (global->_enabled) {
            for (int channelIndex = 0; channelIndex < 3; channelIndex++) {
                affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::ProfileTypes);
                affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::ProfileType);
                affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::RangeTypes);
                affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::RangeType);
            }
        }

        return affectedIndices;
    }},
    { Global::Column::RangeType, [](Global* global, const QVariant& value, const QModelIndex& index) {
        global->_profile.setRangeType(Profile::getRangeTypeEnum(value.toString()));
        
        QModelIndexList affectedIndices;

        if (global->_enabled) {
            for (int channelIndex = 0; channelIndex < 3; channelIndex++) {
                affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::ProfileTypes);
                affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::ProfileType);
                affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::RangeTypes);
                affectedIndices << Global::getChannelModelIndex(index, channelIndex, Channel::Column::RangeType);
            }
        }

        return affectedIndices;
    }}
};

Global::Global(ModelItem* parent) :
    ModelItem("Global", parent),
    _enabled(true),
    _profile(Profile::ProfileType::Mean)
{
}

int Global::columnCount() const 
{
    return Column::End + 1;
}

Qt::ItemFlags Global::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags;

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
            if (_enabled)
                flags |= Qt::ItemIsEnabled;

            break;
        }

        case Column::RangeTypes:
            break;

        case Column::RangeType: {
            if (_enabled && !_profile.getRangeTypes().isEmpty())
                flags |= Qt::ItemIsEnabled;

            break;
        }

        default:
            break;
    }

    return flags;
}

QVariant Global::getData(const QModelIndex& index, const int& role) const
{
    const auto column = index.column();

    switch (role)
    {
        case Qt::EditRole:
        {
            if (getEditRoles.contains(static_cast<Column>(column)))
                return getEditRoles[static_cast<Column>(column)](const_cast<Global*>(this));

            break;
        }

        case Qt::DisplayRole:
        {
            if (getDisplayRoles.contains(static_cast<Column>(column)))
                return getDisplayRoles[static_cast<Column>(column)](const_cast<Global*>(this));

            break;
        }

        default:
            break;
    }

    return QVariant();
}

QModelIndexList Global::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    const auto column = static_cast<Column>(index.column());

    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole:
        {
            if (setEditRoles.contains(column))
                affectedIndices << setEditRoles[column](const_cast<Global*>(this), value, index);

            break;
        }

        default:
            break;
    }

    return affectedIndices;
}

ModelItem* Global::getChild(const int& index) const
{
    return nullptr;
}

int Global::getChildCount() const
{
    return 0;
}

int Global::getChildIndex(ModelItem* child) const
{
    return 0;
}

QModelIndex Global::getConfigurationModelIndex(const QModelIndex& index)
{
    return index.parent();
}

QModelIndex Global::getChannelsModelIndex(const QModelIndex& index)
{
    return getConfigurationsModel()->index(0, 0, getConfigurationModelIndex(index));
}

QModelIndex Global::getChannelModelIndex(const QModelIndex& index, const std::int32_t& row, const std::int32_t& column)
{
    return getConfigurationsModel()->index(row, column, getChannelsModelIndex(index));
}