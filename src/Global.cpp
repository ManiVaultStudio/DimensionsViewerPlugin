#include "Global.h"
#include "Configuration.h"
#include "Channel.h"
#include "DimensionsViewerPlugin.h"
#include "ConfigurationsModel.h"

#include <QDebug>

const QMap<QString, Global::Columns> Global::columns = {
    { "Name", Global::Columns::Name },
    { "Enabled", Global::Columns::Enabled },
    { "Profile types", Global::Columns::ProfileTypes },
    { "Profile type", Global::Columns::ProfileType },
    { "Range types", Global::Columns::RangeTypes },
    { "Range type", Global::Columns::RangeType }
};

const QMap<Global::Columns, std::function<QVariant(Global* global)>> Global::getEditRoles = {
    { Global::Columns::Name, [](Global* global) {
        return global->_name;
    }},
    { Global::Columns::Enabled, [](Global* global) {
        return global->_enabled;
    }},
    { Global::Columns::ProfileTypes, [](Global* global) {
        return global->_profile.getProfileTypeNames();
    }},
    { Global::Columns::ProfileType, [](Global* global) {
        return static_cast<int>(global->_profile.getProfileType());
    }},
    { Global::Columns::RangeTypes, [](Global* global) {
        return global->_profile.getRangeTypeNames();
    }},
    { Global::Columns::RangeType, [](Global* global) {
        return static_cast<int>(global->_profile.getRangeType());
    }}
};

const QMap<Global::Columns, std::function<QVariant(Global* global)>> Global::getDisplayRoles = {
    { Global::Columns::Name, [](Global* global) {
        return global->_name;
    }},
    { Global::Columns::Enabled, [](Global* global) {
        return global->_enabled ? "on" : "off";
    }},
    { Global::Columns::ProfileTypes, [](Global* global) {
        return global->_profile.getProfileTypeNames().join(",");
    }},
    { Global::Columns::ProfileType, [](Global* global) {
        return Profile::getProfileTypeName(global->_profile.getProfileType());
    }},
    { Global::Columns::RangeTypes, [](Global* global) {
        return global->_profile.getRangeTypeNames().join(",");
    }},
    { Global::Columns::RangeType, [](Global* global) {
        return Profile::getRangeTypeName(global->_profile.getRangeType());
    }}
};

const QMap<Global::Columns, std::function<QModelIndexList(Global* global, const QVariant& value, const QModelIndex& index)>> Global::setEditRoles = {
    { Global::Columns::Enabled, [](Global* global, const QVariant& value, const QModelIndex& index) {
        global->_enabled = value.toBool();

        QModelIndexList affectedIndices;

        affectedIndices << index.siblingAtColumn(to_ul(Global::Columns::ProfileTypes));
        affectedIndices << index.siblingAtColumn(to_ul(Global::Columns::ProfileType));
        affectedIndices << index.siblingAtColumn(to_ul(Global::Columns::RangeTypes));
        affectedIndices << index.siblingAtColumn(to_ul(Global::Columns::RangeType));
        
        const auto configurationIndex   = index.parent();
        const auto channelsIndex        = global->index(0, 0, configurationIndex);

        QVector<std::int32_t> channels;

        channels << Channels::Rows::Channel1;
        channels << Channels::Rows::Channel2;
        channels << Channels::Rows::Channel3;

        for (auto channel : channels) {
            const auto channelIndex = global->index(channel, 0, channelsIndex);

            affectedIndices << channelIndex.siblingAtColumn(to_ul(Channel::Columns::ProfileTypes));
            affectedIndices << channelIndex.siblingAtColumn(to_ul(Channel::Columns::ProfileType));
            affectedIndices << channelIndex.siblingAtColumn(to_ul(Channel::Columns::RangeTypes));
            affectedIndices << channelIndex.siblingAtColumn(to_ul(Channel::Columns::RangeType));
        }

        return affectedIndices;
    }},
    { Global::Columns::ProfileType, [](Global* global, const QVariant& value, const QModelIndex& index) {
        global->_profile.setProfileType(Profile::getProfileTypeEnum(value.toString()));

        QModelIndexList affectedIndices;

        affectedIndices << index.siblingAtColumn(to_ul(Global::Columns::RangeTypes));
        affectedIndices << index.siblingAtColumn(to_ul(Global::Columns::RangeType));

        const auto configurationIndex = index.parent();
        const auto channelsIndex = global->index(0, 0, configurationIndex);

        QVector<std::int32_t> channels;

        channels << Channels::Rows::Channel1;
        channels << Channels::Rows::Channel2;
        channels << Channels::Rows::Channel3;

        for (auto channel : channels) {
            const auto channelIndex = global->index(channel, 0, channelsIndex);

            affectedIndices << channelIndex.siblingAtColumn(to_ul(Channel::Columns::ProfileTypes));
            affectedIndices << channelIndex.siblingAtColumn(to_ul(Channel::Columns::ProfileType));
            affectedIndices << channelIndex.siblingAtColumn(to_ul(Channel::Columns::RangeTypes));
            affectedIndices << channelIndex.siblingAtColumn(to_ul(Channel::Columns::RangeType));
        }

        return affectedIndices;
    }},
    { Global::Columns::RangeType, [](Global* global, const QVariant& value, const QModelIndex& index) {
        global->_profile.setRangeType(Profile::getRangeTypeEnum(value.toString()));
        
        QModelIndexList affectedIndices;

        const auto configurationIndex = index.parent();
        const auto channelsIndex = global->index(0, 0, configurationIndex);

        QVector<std::int32_t> channels;

        channels << Channels::Rows::Channel1;
        channels << Channels::Rows::Channel2;
        channels << Channels::Rows::Channel3;

        for (auto channel : channels) {
            const auto channelIndex = global->index(channel, 0, channelsIndex);

            affectedIndices << channelIndex.siblingAtColumn(to_ul(Channel::Columns::ProfileTypes));
            affectedIndices << channelIndex.siblingAtColumn(to_ul(Channel::Columns::ProfileType));
            affectedIndices << channelIndex.siblingAtColumn(to_ul(Channel::Columns::RangeTypes));
            affectedIndices << channelIndex.siblingAtColumn(to_ul(Channel::Columns::RangeType));
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
    return ModelItem::maxNoColumns;
}

Qt::ItemFlags Global::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags;

    const auto column = static_cast<Columns>(index.column());

    switch (column)
    {
        case Columns::Enabled: {
            flags |= Qt::ItemIsEnabled;
            break;
        }

        case Columns::ProfileTypes:
            break;

        case Columns::ProfileType: {
            if (_enabled)
                flags |= Qt::ItemIsEnabled;

            break;
        }

        case Columns::RangeTypes:
            break;

        case Columns::RangeType: {
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
            if (getEditRoles.contains(static_cast<Columns>(column)))
                return getEditRoles[static_cast<Columns>(column)](const_cast<Global*>(this));

            break;
        }

        case Qt::DisplayRole:
        {
            if (getDisplayRoles.contains(static_cast<Columns>(column)))
                return getDisplayRoles[static_cast<Columns>(column)](const_cast<Global*>(this));

            break;
        }

        default:
            break;
    }

    return QVariant();
}

QModelIndexList Global::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    const auto column = static_cast<Columns>(index.column());

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