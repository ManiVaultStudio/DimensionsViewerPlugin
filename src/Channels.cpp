#include "Channels.h"
#include "Channel.h"
#include "Configuration.h"

#include <QDebug>

Channels::Channels(ModelItem* parent, const QString& datasetName, const QString& dataName) :
    ModelItem(parent),
    _channels({
        new Channel(this, 0, "Dataset", true, datasetName, dataName, Qt::black, 0.25f),
        new Channel(this, 1, "Subset 1", false, "", dataName, QColor(249, 149, 0), 0.25f),
        new Channel(this, 2, "Subset 2", false, "", dataName, QColor(0, 112, 249), 0.25f)
    })
{
}

Qt::ItemFlags Channels::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    return flags;
}

QVariant Channels::getData(const QModelIndex& index, const int& role) const
{
    return getData(index.column(), role);
}

QVariant Channels::getData(const int& column, const int& role) const
{
    switch (static_cast<Column>(column))
    {
        default:
            break;
    }

    return QVariant();
}

QModelIndexList Channels::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    return QModelIndexList();
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

Configuration* Channels::getConfiguration() const
{
    return dynamic_cast<Configuration*>(parent());
}

Channel* Channels::getChannelByDatasetName(const QString& datasetName)
{
    for (auto channel : _channels) {
        if (datasetName == channel->getData(Channel::DatasetName, Qt::EditRole).toString())
            return channel;
    }

    return nullptr;
}

QVector<std::uint32_t> Channels::getChannelsEnabled() const
{
    QVector<std::uint32_t> channelsEnabled;

    for (auto channel : _channels) {
        if (channel->getData(Channel::Enabled, Qt::EditRole).toBool())
            channelsEnabled << channel->getData(Channel::Index, Qt::EditRole).toInt();
    }

    return channelsEnabled;
}

std::int32_t Channels::getNoChannelsEnabled() const
{
    auto noChannelsEnabled = 0;

    for (auto channel : _channels) {
        if (channel->getData(Channel::Enabled, Qt::EditRole).toBool())
            noChannelsEnabled++;
    }

    return noChannelsEnabled;
}

std::int32_t Channels::getNoDisplayChannels() const
{
    auto noDisplayChannels = 0;

    for (auto channel : _channels) {
        if (channel->canDisplay())
            noDisplayChannels++;
    }

    return noDisplayChannels;
}