#include "Differential.h"
#include "Channels.h"
#include "Channel.h"
#include "Profile.h"

#include <QDebug>

Differential::Differential(Channel* channel) :
    _channel(channel),
    _operandChannelNames(),
    _operandChannelName()
{
}

QStringList Differential::getOperandChannelNames(const Operand& operand) const
{
    return _operandChannelNames[static_cast<std::int32_t>(operand)];
}

QString Differential::getOperandChannelName(const Operand& operand) const
{
    return _operandChannelName[static_cast<std::int32_t>(operand)];
}

bool Differential::isPrimed() const
{
    return getCandidateChannelNames().count() >= 2;
}

bool Differential::isValid() const
{
    if (_operandChannelName[to_ul(Operand::ChannelA)] == "")
        return false;

    if (_operandChannelName[to_ul(Operand::ChannelB)] == "")
        return false;

    return true;
}

QStringList Differential::getCandidateChannelNames() const
{
    auto enabled = true;

    const auto candidateChannels = _channel->getChannels()->getFiltered(Profile::ProfileTypes({Profile::ProfileType::Mean, Profile::ProfileType::Median }), &enabled);

    QStringList candidateChannelNames;

    for (auto candidateChannel : candidateChannels)
        candidateChannelNames << candidateChannel->getData(Channel::Column::DatasetName, Qt::EditRole).toString();

    return candidateChannelNames;
}

void Differential::update()
{
    const auto candidateChannelNames = getCandidateChannelNames();

    switch (candidateChannelNames.count())
    {
        case 1:
        {
            _operandChannelNames[to_ul(Operand::ChannelA)] = QStringList();
            _operandChannelNames[to_ul(Operand::ChannelB)] = QStringList();

            _operandChannelName[to_ul(Operand::ChannelA)] = "";
            _operandChannelName[to_ul(Operand::ChannelB)] = "";

            break;
        }

        case 2:
        {
            _operandChannelNames[to_ul(Operand::ChannelA)] = QStringList(candidateChannelNames[0]);
            _operandChannelNames[to_ul(Operand::ChannelB)] = QStringList(candidateChannelNames[1]);

            _operandChannelName[to_ul(Operand::ChannelA)] = candidateChannelNames[0];
            _operandChannelName[to_ul(Operand::ChannelB)] = candidateChannelNames[1];

            break;
        }

        default:
            break;
    }
}