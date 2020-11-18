#include "Profile.h"
#include "Channels.h"
#include "Channel.h"
#include "Visitor.h"
#include "ConfigurationsModel.h"

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
    { "MinMax", Profile::RangeType::MinMax },
    { "5/95 perc.", Profile::RangeType::Percentile5 },
    { "10/90 perc.", Profile::RangeType::Percentile10 }
};

Profile::Profile(TreeItem* parent /*= nullptr*/, const ProfileType& profileType /*= ProfileType::Mean*/) :
    TreeItem("Profile", "Profile", parent),
    _profileTypes(),
    _profileType(profileType),
    _rangeType(RangeType::MinMax),
    _rangeTypes()
{
    setNumColumns(to_ul(Column::_Count));

    switch (_profileType)
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

    update();
}

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

QVariant Profile::getData(const std::int32_t& column, const std::int32_t& role) const
{
    auto data = TreeItem::getData(column, role);

    switch (role)
    {
        case Qt::EditRole:
        {
            switch (static_cast<Column>(column))
            {
                case Column::ProfileTypes:
                {
                    QStringList profileTypeNames;

                    for (auto profileType : _profileTypes)
                        profileTypeNames << getProfileTypeName(profileType);

                    return profileTypeNames;
                }

                case Column::ProfileType:
                    return static_cast<std::int32_t>(_profileType);

                case Column::RangeTypes:
                {
                    QStringList rangeTypeNames;

                    for (auto rangeType : _rangeTypes)
                        rangeTypeNames << getRangeTypeName(rangeType);

                    return rangeTypeNames;
                }

                case Column::RangeType:
                    return static_cast<std::int32_t>(_rangeType);

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole:
        {
            switch (static_cast<Column>(column))
            {
                case Column::ProfileTypes:
                    return getData(column, Qt::EditRole).toStringList().join(", ");

                case Column::ProfileType:
                    return Profile::getProfileTypeName(static_cast<Profile::ProfileType>(getData(column, Qt::EditRole).toInt()));

                case Column::RangeTypes:
                    return getData(column, Qt::EditRole).toStringList().join(", ");

                case Column::RangeType:
                    return Profile::getRangeTypeName(static_cast<Profile::RangeType>(getData(column, Qt::EditRole).toInt()));

                default:
                    break;
            }

            break;
        }

        case Qt::ToolTipRole:
        {
            const auto tooltip = [&column](const QString& value) {
                return QString("%1: %2").arg(getColumnTypeName(static_cast<Column>(column)), value);
            };

            switch (static_cast<Column>(column))
            {
                case Column::ProfileTypes:
                case Column::ProfileType:
                case Column::RangeTypes:
                case Column::RangeType:
                    return tooltip(getData(column, Qt::DisplayRole).toString());

                default:
                    break;
            }

            break;
        }

        default:
            break;
    }

    return data;
}

QVariant Profile::getData(const Column& column, const std::int32_t& role) const
{
    return getData(static_cast<std::int32_t>(column), role);
}

void Profile::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    TreeItem::setData(index, value, role);

    const auto column = static_cast<Column>(index.column());

    switch (role)
    {
        case Qt::EditRole:
        {
            switch (column)
            {
                case Column::ProfileTypes:
                {
                    _profileTypes.clear();

                    for (auto profileType : value.toStringList())
                        _profileTypes << getProfileTypeEnum(profileType);

                    break;
                }

                case Column::ProfileType:
                {
                    _profileType = static_cast<Profile::ProfileType>(value.toInt());
                    break;
                }

                case Column::RangeTypes:
                {
                    _rangeTypes.clear();

                    for (auto rangeType : value.toStringList())
                        _rangeTypes << getRangeTypeEnum(rangeType);

                    break;
                }

                case Column::RangeType:
                {
                    _rangeType = static_cast<Profile::RangeType>(value.toInt());
                    break;
                }

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole:
        {
            switch (column)
            {
                case Column::ProfileTypes:
                    break;

                case Column::ProfileType:
                {
                    _profileType = Profile::getProfileTypeEnum(value.toString());
                    break;
                }

                case Column::RangeTypes:
                    break;

                case Column::RangeType:
                {
                    _rangeType = Profile::getRangeTypeEnum(value.toString());
                    break;
                }

                default:
                    break;
            }
            break;
        }

        default:
            break;
    }
}

void Profile::accept(Visitor* visitor) const
{
    visitor->visitProfile(this);
}

void Profile::setProfile(const Profile* profile)
{
    Columns copyColumns = {
        Column::ProfileTypes,
        Column::ProfileType,
        Column::RangeTypes,
        Column::RangeType
    };

    for (auto copyColumn : copyColumns)
        getModel()->setData(getSiblingAtColumn(to_ul(copyColumn)), profile->getData(copyColumn, Qt::EditRole), Qt::EditRole);
}

void Profile::update()
{
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
}

const Channel* Profile::getChannel() const
{
    return dynamic_cast<Channel*>(_parent);
}