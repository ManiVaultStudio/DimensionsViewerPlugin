#include "Channels.h"
#include "Channel.h"

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

int Channels::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    return 4;
}

Qt::ItemFlags Channels::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEditable;
    return flags;
}

QVariant Channels::getData(const QModelIndex& index, const int& role) const
{
    return "CHANNELS_MODEL_ITEM";
}

QModelIndexList Channels::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    return QModelIndexList();
}

QModelIndex Channels::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
    return QModelIndex();
}

QModelIndex Channels::parent(const QModelIndex& index) const
{
    return QModelIndex();
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

Channel* Channels::getChannelByDatasetName(const QString& datasetName)
{
    for (auto channel : _channels) {
        if (datasetName == channel->getDatasetName())
            return channel;
    }

    return nullptr;
}

QVector<std::uint32_t> Channels::getChannelsEnabled() const
{
    QVector<std::uint32_t> channelsEnabled;

    for (auto channel : _channels) {
        if (channel->isEnabled())
            channelsEnabled << channel->getIndex();
    }

    return channelsEnabled;
}

std::int32_t Channels::getNoChannelsEnabled() const
{
    auto noChannelsEnabled = 0;

    for (auto channel : _channels) {
        if (channel->isEnabled())
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