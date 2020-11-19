#include "Profile.h"
#include "Channel.h"
#include "StandardItems.h"
#include "Visitor.h"

#include <QDebug>

const QMap<QString, Profile::Column> Profile::columns = {
    { "Profile types", Profile::Column::ProfileTypes },
    { "Profile type", Profile::Column::ProfileType },
    { "Range types", Profile::Column::RangeTypes },
    { "Range type", Profile::Column::RangeType }
};

const QMap<QString, Profile::ProfileType> Profile::profileTypes = {
    { "None", Profile::ProfileType::None },
    { "Mean", Profile::ProfileType::Mean },
    { "Median", Profile::ProfileType::Median },
    { "Differential", Profile::ProfileType::Differential }
};

const QMap<QString, Profile::RangeType> Profile::rangeTypes = {
    { "None", Profile::RangeType::None },
    { "1 StdDev", Profile::RangeType::StandardDeviation1 },
    { "2 StdDev", Profile::RangeType::StandardDeviation2 },
    { "3 StdDev", Profile::RangeType::StandardDeviation3 },
    { "Min/Max", Profile::RangeType::MinMax },
    { "5/95 perc.", Profile::RangeType::Percentile5 },
    { "10/90 perc.", Profile::RangeType::Percentile10 }
};

Profile::Profile(Item* parent /*= nullptr*/, const ProfileType& profileType /*= ProfileType::Mean*/) :
    Item(parent, "Profile", "Profile")
{
    _flags.setFlag(Qt::ItemIsEditable);
    _flags.setFlag(Qt::ItemIsEnabled);
    _flags.setFlag(Qt::ItemIsSelectable);

    _children << new tree::StringList(this, "Profile types", QStringList(profileTypes.keys()));
    _children << new tree::String(this, "Profile type", "Mean");
    _children << new tree::StringList(this, "Range types", QStringList(rangeTypes.keys()));
    _children << new tree::String(this, "Range type", "Min/Max");

    /*switch (_profileType)
    {
        case ProfileType::Mean:
        case ProfileType::Median:
        {
            _profileTypes << ProfileType::Mean << ProfileType::Median;
            break;
        }

        case ProfileType::Differential:
        {
            _profileTypes << ProfileType::Differential;
            break;
        }

        default:
            break;
    }

    QObject::connect(this, &Profile::dataChanged, [this](const QModelIndex& modelIndex) {
        switch (static_cast<Column>(modelIndex.column()))
        {
            case Column::ProfileTypes:
            case Column::ProfileType:
            {
                update();

                Columns updateColumns = {
                    Column::RangeTypes,
                    Column::RangeType
                };

                for (auto updateColumn : updateColumns)
                    emit getModel()->dataChanged(modelIndex.siblingAtColumn(to_ul(updateColumn)), modelIndex.siblingAtColumn(to_ul(updateColumn)));

                break;
            }

            default:
                break;
        }
    });

    update();*/
}

/*
Qt::ItemFlags Profile::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = TreeItem::getFlags(index);

    const auto column = static_cast<Column>(index.column());
    
    auto channel = dynamic_cast<Channel*>(_parent);

    switch (column)
    {
        case Column::ProfileType:
        case Column::RangeTypes:
        case Column::RangeType:
        {
            flags |= Qt::ItemIsEditable;

            if (channel->_enabled) {
                switch (static_cast<Channels::Row>(channel->_index))
                {
                    case Channels::Row::Dataset:
                    {
                        flags |= Qt::ItemIsEnabled;

                        break;
                    }

                    case Channels::Row::Subset1:
                    case Channels::Row::Subset2:
                    {
                        const auto meanOrMedian = _profileType == Profile::ProfileType::Mean || _profileType == Profile::ProfileType::Median;

                        if (!channel->_linked && meanOrMedian)
                            flags |= Qt::ItemIsEnabled;

                        break;
                    }

                    case Channels::Row::Differential:
                        break;

                    default:
                        break;
                }
            }

            break;
        }

        default:
            break;
    }

    return flags;
}
*/

void Profile::accept(tree::Visitor* visitor) const
{
    visitor->visitTreeItem(this);
}

void Profile::setProfile(const Profile* profile)
{
    Columns copyColumns = {
        Column::ProfileTypes,
        Column::ProfileType,
        Column::RangeTypes,
        Column::RangeType
    };

    //for (auto copyColumn : copyColumns)
    //    getModel()->setData(getSiblingAtColumn(to_ul(copyColumn)), profile->getData(copyColumn, Qt::EditRole), Qt::EditRole);
}

void Profile::update()
{
    /*
    _rangeTypes.clear();

    switch (_profileType)
    {
        case ProfileType::None:
        case ProfileType::Differential:
        {
            _rangeType = RangeType::None;

            break;
        }

        case ProfileType::Mean:
        {
            _rangeTypes << RangeType::None;
            _rangeTypes << RangeType::StandardDeviation1;
            _rangeTypes << RangeType::StandardDeviation2;
            _rangeTypes << RangeType::StandardDeviation3;
            _rangeTypes << RangeType::MinMax;

            if (!_rangeTypes.contains(_rangeType))
                _rangeType = RangeType::StandardDeviation1;

            break;
        }

        case ProfileType::Median:
        {
            _rangeTypes << RangeType::Percentile5;
            _rangeTypes << RangeType::Percentile10;

            if (!_rangeTypes.contains(_rangeType))
                _rangeType = RangeType::Percentile5;

            break;
        }

        default:
            break;
    }
    */
}

const Channel* Profile::getChannel() const
{
    return dynamic_cast<Channel*>(_parent);
}