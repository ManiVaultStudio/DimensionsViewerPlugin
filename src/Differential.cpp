#include "Differential.h"
#include "Channels.h"
#include "Channel.h"
#include "Profile.h"

#include <QDebug>

Differential::Differential(Channel* channel) :
    _channel(channel),
    _channelNames(),
    _channelName()
{
}

QStringList Differential::getChannelNames(const Operand& operand) const
{
    return _channelNames[static_cast<std::int32_t>(operand)];
}

QString Differential::getChannelName(const Operand& operand) const
{
    return _channelName[static_cast<std::int32_t>(operand)];
}

QStringList Differential::getCandidateChannelNames() const
{
    QStringList candidateChannelNames;

    for (auto channel : _channel->getChannels()->_channels) {
        if (!channel->_enabled)
            continue;

        const auto profileType = channel->_profile.getProfileType();

        if (profileType != Profile::ProfileType::None)
            continue;

        if (profileType != Profile::ProfileType::Differential)
            continue;

        candidateChannelNames << channel->_displayName;
    }

    return candidateChannelNames;
}