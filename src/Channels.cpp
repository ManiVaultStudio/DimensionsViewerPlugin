#include "Channels.h"
#include "Channel.h"
#include "Configuration.h"

#include <QDebug>

const QMap<QString, Channels::Column> Channels::columns = {
    { "Name", Channels::Column::Name }
};

const QMap<Channels::Column, std::function<QVariant(Channels* channels)>> Channels::getEditRoles = {
    { Channels::Column::Name, [](Channels* channels) {
        return channels->_name;
    }}
};

const QMap<Channels::Column, std::function<QVariant(Channels* channels)>> Channels::getDisplayRoles = {
    { Channels::Column::Name, [](Channels* channels) {
        return getEditRoles[Channels::Column::Name](channels);
    }}
};

const QMap<Channels::Column, std::function<QModelIndexList(Channels* channel, const QVariant& value, const QModelIndex& index)>> Channels::setEditRoles = {};

Channels::Channels(ModelItem* parent, const QString& datasetName, const QString& dataName) :
    ModelItem("Channels", parent),
    _channels({
        new Channel(this, 0, "Dataset", true, datasetName, dataName, Qt::black, 0.25f),
        new Channel(this, 1, "Subset 1", false, "", dataName, QColor(249, 149, 0), 0.25f),
        new Channel(this, 2, "Subset 2", false, "", dataName, QColor(0, 112, 249), 0.25f)
    })
{
}

int Channels::columnCount() const
{
    return ModelItem::maxNoColumns;
}

Qt::ItemFlags Channels::getFlags(const QModelIndex& index) const
{
    return Qt::NoItemFlags;
}

QVariant Channels::getData(const QModelIndex& index, const int& role) const
{
    const auto column = index.column();

    switch (role)
    {
        case Qt::EditRole:
        {
            if (getEditRoles.contains(static_cast<Column>(column)))
                return getEditRoles[static_cast<Column>(column)](const_cast<Channels*>(this));

            break;
        }

        case Qt::DisplayRole:
        {
            if (getDisplayRoles.contains(static_cast<Column>(column)))
                return getDisplayRoles[static_cast<Column>(column)](const_cast<Channels*>(this));

            break;
        }

        default:
            break;
    }

    return QVariant();
}

QModelIndexList Channels::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    const auto column = static_cast<Column>(index.column());

    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole:
        {
            if (setEditRoles.contains(column))
                affectedIndices << setEditRoles[column](const_cast<Channels*>(this), value, index);

            break;
        }

        default:
            break;
    }

    return affectedIndices;
}

ModelItem* Channels::getChild(const int& index) const
{
    try
    {
        return _channels[index];
    }
    catch (std::exception exception)
    {
        return nullptr;
    }
}

int Channels::getChildCount() const
{
    return _channels.size();
}

int Channels::getChildIndex(ModelItem* child) const
{
    const auto channel = dynamic_cast<Channel*>(child);

    if (channel == nullptr)
        return 0;

    return _channels.indexOf(channel);
}

const Configuration* Channels::getConfiguration() const
{
    return dynamic_cast<Configuration*>(parent());
}

std::int32_t Channels::getNoChannelsEnabled() const
{
    auto noChannelsEnabled = 0;

    for (auto channel : _channels)
        if (channel->_enabled)
            noChannelsEnabled++;

    return noChannelsEnabled;
}