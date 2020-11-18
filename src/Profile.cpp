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
    TreeItem(getModel()->index(to_ul(Channel::Row::Profile), 0, parent->getModelIndex()), "Profile", "Profile", parent),
    _locked(profileType == ProfileType::Differential),
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
                        const auto meanOrMedian = getProfileType() == Profile::ProfileType::Mean || getProfileType() == Profile::ProfileType::Median;

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
                    return getProfileTypeNames();

                case Column::ProfileType:
                    return static_cast<std::int32_t>(getProfileType());

                case Column::RangeTypes:
                    return getRangeTypeNames();

                case Column::RangeType:
                    return static_cast<std::int32_t>(getRangeType());

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

QModelIndexList Profile::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    QModelIndexList affectedIndices = TreeItem::setData(index, value, role) << getAffectedIndices(index);

    const auto column = static_cast<Column>(index.column());

    switch (role)
    {
        case Qt::EditRole:
        {
            switch (column)
            {
                case Column::ProfileTypes:
                    break;

                case Column::ProfileType:
                    setProfileType(static_cast<Profile::ProfileType>(value.toInt()));
                    break;

                case Column::RangeTypes:
                    break;

                case Column::RangeType:
                    setRangeType(static_cast<Profile::RangeType>(value.toInt()));
                    break;

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
                    setProfileType(Profile::getProfileTypeEnum(value.toString()));
                    break;

                case Column::RangeTypes:
                    break;

                case Column::RangeType:
                    setRangeType(Profile::getRangeTypeEnum(value.toString()));
                    break;

                default:
                    break;
            }
            break;
        }

        default:
            break;
    }

    /*const auto channel = index.siblingAtColumn(0).parent();

    if (channel.row() == to_ul(Channels::Row::Dataset)) {
        Channels::Rows updateRows{
            Channels::Row::Subset1,
            Channels::Row::Subset2
        };

        for (auto updateRow : updateRows) {
            const auto otherChannel = channel.siblingAtRow(to_ul(updateRow));

            if (!otherChannel.siblingAtColumn(to_ul(Channel::Column::Linked)).data(Qt::EditRole).toBool())
                continue;

            const auto otherProfile = getModel()->index(to_ul(Channel::Row::Profile), 0, otherChannel);

            Columns updateColumns {
                Column::ProfileTypes,
                Column::ProfileType,
                Column::RangeTypes,
                Column::RangeType
            };

            for (auto updateColumn : updateColumns)
                getModel()->setData(otherProfile.siblingAtColumn(to_ul(updateColumn)), getData(updateColumn, Qt::EditRole));
        }
    }*/

    return affectedIndices;
}

QModelIndexList Profile::getAffectedIndices(const QModelIndex& index) const
{
    QModelIndexList affectedIndices{ index };
    
    const auto column = static_cast<Column>(index.column());

    switch (column)
    {
        case Column::ProfileTypes:
        {
            break;
        }

        case Column::ProfileType:
        {
            affectedIndices << index.siblingAtColumn(to_ul(Column::RangeTypes));
            affectedIndices << index.siblingAtColumn(to_ul(Column::RangeType));

            break;
        }

        case Column::RangeTypes:
        {
            break;
        }

        case Column::RangeType:
        {
            break;
        }

        default:
            break;
    }

    //const auto channel = index.siblingAtColumn(0).parent();

    //if (channel.row() == to_ul(Channels::Row::Dataset)) {
    //    Channels::Rows updateRows{
    //        Channels::Row::Subset1,
    //        Channels::Row::Subset2
    //    };

    //    for (auto row : updateRows) {
    //        affectedIndices << getModel()->index(to_ul(row), to_ul(Column::ProfileTypes), channel);
    //        affectedIndices << getModel()->index(to_ul(row), to_ul(Column::ProfileType), channel);
    //        affectedIndices << getModel()->index(to_ul(row), to_ul(Column::RangeTypes), channel);
    //        affectedIndices << getModel()->index(to_ul(row), to_ul(Column::RangeType), channel);
    //    }
    //}

    return affectedIndices;
}

void Profile::accept(Visitor* visitor) const
{
    visitor->visitProfile(this);
}

Profile::ProfileTypes Profile::getProfileTypes() const
{
    return _profileTypes;
}

QStringList Profile::getProfileTypeNames() const
{
    QStringList profileTypeNames;

    for (auto profileType : _profileTypes)
        profileTypeNames << getProfileTypeName(profileType);

    return profileTypeNames;

}
Profile::ProfileType Profile::getProfileType() const
{
    return _profileType;
}

void Profile::setProfileType(const ProfileType& profileType)
{
    if (_locked)
        return;

    if (profileType == _profileType)
        return;

    _profileType = profileType;

    update();
}

Profile::RangeTypes Profile::getRangeTypes() const
{
    return _rangeTypes;
}

QStringList Profile::getRangeTypeNames() const
{
    QStringList rangeTypeNames;

    for (auto rangeType : _rangeTypes)
        rangeTypeNames << getRangeTypeName(rangeType);

    return rangeTypeNames;
}

Profile::RangeType Profile::getRangeType() const
{
    return _rangeType;
}

void Profile::setRangeType(const RangeType& rangeType)
{
    if (_locked)
        return;

    if (rangeType == _rangeType)
        return;

    if (_rangeTypes.contains(_rangeType))
        _rangeType = rangeType;
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