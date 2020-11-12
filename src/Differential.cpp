#include "Differential.h"
#include "Channels.h"
#include "Channel.h"
#include "Profile.h"

#include <QDebug>

Differential::Differential(Channel* channel) :
    _channel(channel),
    _operandNames(),
    _operandName()
{
}

QStringList Differential::getChannelNames(const Operand& operand) const
{
    return _operandNames[static_cast<std::int32_t>(operand)];
}

QString Differential::getChannelName(const Operand& operand) const
{
    return _operandName[static_cast<std::int32_t>(operand)];
}

QStringList Differential::getCandidateChannelNames() const
{
    QStringList candidateChannelNames;

    for (auto channel : _channel->getChannels()->_channels) {
        if (!channel->_enabled)
            continue;

        const auto profileType = channel->_profile.getProfileType();

        if (profileType == Profile::ProfileType::None || profileType == Profile::ProfileType::Differential)
            continue;

        candidateChannelNames << channel->_displayName;
    }

    return candidateChannelNames;
}

void Differential::update()
{
    const auto candidateChannelNames = getCandidateChannelNames();

    switch (candidateChannelNames.count())
    {
        case 1:
        {
            _operandNames[to_ul(Operand::ChannelA)] = QStringList();
            _operandNames[to_ul(Operand::ChannelB)] = QStringList();

            _operandName[to_ul(Operand::ChannelA)] = "";
            _operandName[to_ul(Operand::ChannelB)] = "";

            break;
        }

        case 2:
        {
            _operandNames[to_ul(Operand::ChannelA)] = QStringList(candidateChannelNames[0]);
            _operandNames[to_ul(Operand::ChannelB)] = QStringList(candidateChannelNames[1]);

            _operandName[to_ul(Operand::ChannelA)] = candidateChannelNames[0];
            _operandName[to_ul(Operand::ChannelB)] = candidateChannelNames[1];

            break;
        }

        default:
            break;
    }

    _valid = candidateChannelNames.count() >= 2;
}