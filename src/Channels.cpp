#include "Channels.h"
#include "Channel.h"

#include <QDebug>

Channels::Channels(QObject* parent, const QString& datasetName, const QString& dataName) :
    QObject(parent),
    _channels({
        new Channel(this, 0, "Dataset", true, datasetName, dataName, Qt::black, 0.25f),
        new Channel(this, 1, "Subset 1", false, "", dataName, QColor(249, 149, 0), 0.25f),
        new Channel(this, 2, "Subset 2", false, "", dataName, QColor(0, 112, 249), 0.25f)
    })
{
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