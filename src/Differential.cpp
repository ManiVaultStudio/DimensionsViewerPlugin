#include "Differential.h"
#include "Visitor.h"

#include <QDebug>

const QMap<QString, Differential::Child> Differential::children = {
    { "Differential operand A names", Differential::Child::DifferentialOperandNamesA },
    { "Differential operand B names", Differential::Child::DifferentialOperandNamesB },
    { "Differential operand A name", Differential::Child::DifferentialOperandA },
    { "Differential operand B name", Differential::Child::DifferentialOperandB }
};

Differential::Differential(Item* parent, const QString& name) :
    Item(parent, "Differential", name),
    _operandChannelNames(),
    _operandChannelName()
{
    _flags.setFlag(Qt::ItemIsEnabled);
    _flags.setFlag(Qt::ItemIsSelectable);
}

//switch (static_cast<Column>(column))
//{
//    case Differential::Column::DifferentialOperandNamesA:
//        return (_channel->_enabled && isPrimed()) ? getOperandChannelNames(Differential::Operand::ChannelA) : QStringList();
//
//    case Differential::Column::DifferentialOperandNamesB:
//        return (_channel->_enabled && isPrimed()) ? getOperandChannelNames(Differential::Operand::ChannelB) : QStringList();
//
//    case Differential::Column::DifferentialOperandA:
//        return (_channel->_enabled && isPrimed()) ? getOperandChannelName(Differential::Operand::ChannelA) : "";
//
//    case Differential::Column::DifferentialOperandB:
//        return (_channel->_enabled && isPrimed()) ? getOperandChannelName(Differential::Operand::ChannelB) : "";
//
//    default:
//        break;
//}

void Differential::accept(tree::Visitor* visitor) const
{
    Q_ASSERT(visitor != nullptr);

    visitor->visitTreeItem(this);
}

QStringList Differential::getOperandChannelNames(const Operand& operand) const
{
    return _operandChannelNames[operand];
}

QString Differential::getOperandChannelName(const Operand& operand) const
{
    return _operandChannelName[operand];
}

void Differential::setOperandChannelName(const Operand& operand, const QString& channelName)
{
    _operandChannelName[operand] = channelName;

    auto candidateChannelNames = getCandidateChannelNames();

    switch (candidateChannelNames.count())
    {
        case 0:
        case 1:
            break;

        case 2:
        case 3:
        {
            _operandChannelNames[Operand::ChannelA] = candidateChannelNames;
            _operandChannelNames[Operand::ChannelB] = candidateChannelNames;

            const auto otherOperand = operand == Operand::ChannelA ? Operand::ChannelB : Operand::ChannelA;

            _operandChannelNames[otherOperand].removeOne(channelName);
            _operandChannelName[otherOperand] = _operandChannelNames[otherOperand].first();
            _operandChannelNames[operand].removeOne(_operandChannelName[otherOperand]);

            break;
        }

        default:
            break;
    }
}

std::int32_t Differential::getNumCombinations() const
{
    std::function<int(int)> factorial;
    
    factorial = [&factorial](int n) {
        return n < 2 ? 1 : n * factorial(n - 1);
    };

    const auto n                = getCandidateChannelNames().count();
    const auto r                = 2;
    const auto numCombinations  = factorial(n) / factorial(2) * factorial(n - 2);

    return numCombinations;
}

bool Differential::isPrimed() const
{
    return getCandidateChannelNames().count() >= 2;
}

bool Differential::isValid() const
{
    if (_operandChannelName[Operand::ChannelA] == "")
        return false;

    if (_operandChannelName[Operand::ChannelB] == "")
        return false;

    return true;
}

QStringList Differential::getCandidateChannelNames() const
{
    auto enabled = true;

    //const auto candidateChannels = _channel->getChannels()->getFiltered(Profile::ProfileTypes({Profile::ProfileType::Mean, Profile::ProfileType::Median }), &enabled);

    QStringList candidateChannelNames;

    //for (auto candidateChannel : candidateChannels)
    //    candidateChannelNames << candidateChannel->getData(Channel::Column::DatasetName, Qt::EditRole).toString();

    return candidateChannelNames;
}

void Differential::update()
{
    const auto candidateChannelNames = getCandidateChannelNames();

    switch (candidateChannelNames.count())
    {
        case 0:
        case 1:
        {
            _operandChannelName[Operand::ChannelA] = "";
            _operandChannelName[Operand::ChannelB] = "";

            _operandChannelNames[Operand::ChannelA] = QStringList();
            _operandChannelNames[Operand::ChannelB] = QStringList();

            break;
        }

        case 2:
        {
            setOperandChannelName(Operand::ChannelA, candidateChannelNames[0]);
            setOperandChannelName(Operand::ChannelB, candidateChannelNames[1]);

            break;
        }

        case 3:
        {
            setOperandChannelName(Operand::ChannelA, candidateChannelNames[1]);
            setOperandChannelName(Operand::ChannelB, candidateChannelNames[2]);

            break;
        }

        default:
            break;
    }
}





//Configuration::AffectedColumns Configuration::updateDifferentialProfile()
//{
//    AffectedColumns affectedColumns;
//
//    const auto resetDifferentialProfile = [this, &affectedColumns]() {
//        affectedColumns << setProfileDatasetName(0, "");
//        affectedColumns << setProfileDatasetName(1, "");
//
//        affectedColumns << setProfileDatasetNames(0, QStringList());
//        affectedColumns << setProfileDatasetNames(1, QStringList());
//    };
//
//    const auto getAllDatasetNames = [this]() -> QStringList {
//        QStringList allDatasetNames;
//
//        for (auto channel : _channels) {
//            if (channel->isEnabled())
//                allDatasetNames << channel->getDatasetName();
//        }
//
//        return allDatasetNames;
//    };
//
//    const auto getDatasetName = [this, &getAllDatasetNames](const std::uint32_t& profileIndex) -> QString {
//        const auto opposingProfileIndex = profileIndex == 0 ? 1 : 0;
//        const auto otherDatasetName = _profileDatasetName[opposingProfileIndex];
//        const auto profileDatasetName = _profileDatasetName[profileIndex];
//
//        auto datasetNames = getAllDatasetNames();
//
//        datasetNames.removeOne(otherDatasetName);
//
//        if (datasetNames.contains(profileDatasetName))
//            return profileDatasetName;
//
//        return datasetNames.first();
//    };
//
//    const auto getDatasetNames = [this, &getAllDatasetNames](const std::uint32_t& profileIndex) -> QStringList {
//        const auto opposingProfileIndex = profileIndex == 0 ? 1 : 0;
//        const auto otherDatasetName = _profileDatasetName[opposingProfileIndex];
//
//        auto datasetNames = getAllDatasetNames();
//
//        datasetNames.removeOne(otherDatasetName);
//
//        return datasetNames;
//    };
//
//    if (!canShowDifferentialProfile()) {
//        resetDifferentialProfile();
//    }
//    else {
//        if (_showDifferentialProfile) {
//            const auto channelsEnabled = getChannelsEnabled();
//            //const auto opposingProfileIndex = profileIndex == 0 ? 1 : 0;
//            //const auto opposingDatasetName  = getChannelDatasetName(opposingProfileIndex, Qt::EditRole).toString();
//
//            switch (channelsEnabled.size())
//            {
//                case 3: {
//                    if (_profileDatasetName[0] == "")
//                        affectedColumns << setProfileDatasetName(0, getChannelDatasetName(1, Qt::EditRole).toString());
//
//                    if (_profileDatasetName[1] == "")
//                        affectedColumns << setProfileDatasetName(1, getChannelDatasetName(2, Qt::EditRole).toString());
//
//                    break;
//                }
//
//                default:
//                    break;
//            }
//
//            affectedColumns << setProfileDatasetNames(0, getDatasetNames(0));
//            affectedColumns << setProfileDatasetNames(1, getDatasetNames(1));
//        }
//        else {
//            resetDifferentialProfile();
//        }
//    }
//
//    return affectedColumns;
//}