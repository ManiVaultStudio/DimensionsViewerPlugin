#include "Profile.h"

#include <QDebug>

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

Profile::Profile(ModelItem* parent, const ProfileType& profileType) :
    ModelItem("Profile", parent),
    _profileType(ProfileType::None),
    _rangeType(RangeType::MinMax),
    _rangeTypes()
{
    setProfileType(ProfileType::Mean);
}

int Profile::columnCount() const
{
    return ModelItem::maxNoColumns;
}

Qt::ItemFlags Profile::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = Qt::ItemIsEditable;

    const auto column = static_cast<Column>(index.column());

    switch (column)
    {
        case Column::Name:
        case Column::ProfileTypes:
        case Column::ProfileType:
        case Column::RangeTypes:
        case Column::RangeType:
        {
            flags |= Qt::ItemIsEnabled;

            break;
        }

        default:
            break;
    }

    return flags;
}

QVariant Profile::getData(const std::int32_t& column, const std::int32_t& role) const
{
    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(column))
            {
                case Profile::Column::Name:
                    return _name;

                case Profile::Column::ProfileTypes:
                    return getProfileTypeNames();

                case Profile::Column::ProfileType:
                    return getProfileTypeName(_profileType);

                case Profile::Column::RangeTypes:
                    return getRangeTypeNames();

                case Profile::Column::RangeType:
                    return getRangeTypeName(_rangeType);

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole: {

            switch (static_cast<Column>(column))
            {
                case Profile::Column::Name:
                    return getData(column, Qt::EditRole);

                case Profile::Column::ProfileTypes:
                    return getData(column, Qt::EditRole).toStringList().join(", ");

                case Profile::Column::ProfileType:
                    return getData(column, Qt::EditRole);

                case Profile::Column::RangeTypes:
                    return getData(column, Qt::EditRole).toStringList().join(", ");

                case Profile::Column::RangeType:
                    return getData(column, Qt::EditRole);

                default:
                    break;
            }

            break;
        }

        default:
            break;
    }

    return QVariant();
}

ModelItem::AffectedColumns Profile::setData(const std::int32_t& column, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    AffectedColumns affectedColunns{ column };

    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(column))
            {
                case Profile::Column::ProfileType: {
                    setProfileType(getProfileTypeEnum(value.toString()));
                    break;
                }

                case Profile::Column::RangeType: {
                    setRangeType(getRangeTypeEnum(value.toString()));
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

    return affectedColunns;
}

Profile::ProfileType Profile::getProfileType() const
{
    return _profileType;
}

void Profile::setProfileType(const ProfileType& profileType)
{
    if (profileType == _profileType)
        return;

    _profileType = profileType;

    _rangeTypes.clear();

    switch (_profileType)
    {
        case ProfileType::None:
        case ProfileType::Differential:
        {
            _rangeType = Profile::RangeType::None;

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
                _rangeType = Profile::RangeType::StandardDeviation1;

            break;
        }

        case ProfileType::Median:
        {
            _rangeTypes << Profile::RangeType::Percentile5;
            _rangeTypes << Profile::RangeType::Percentile10;

            if (!_rangeTypes.contains(_rangeType))
                _rangeType = RangeType::Percentile5;

            break;
        }
        
        default:
            break;
    }
}

Profile::RangeType Profile::getRangeType() const
{
    return _rangeType;
}

void Profile::setRangeType(const RangeType& rangeType)
{
    if (rangeType == _rangeType)
        return;

    if (_rangeTypes.contains(_rangeType))
        _rangeType = rangeType;
}

QStringList Profile::getProfileTypeNames() const
{
    QStringList profileTypeNames;

    for (int i = 1; i <= static_cast<int>(ProfileType::End); ++i)
        profileTypeNames << getProfileTypeName(static_cast<ProfileType>(i));

    return profileTypeNames;
}

QStringList Profile::getRangeTypeNames() const
{
    QStringList rangeTypeNames;

    for (auto rangeType : _rangeTypes)
        rangeTypeNames << getRangeTypeName(rangeType);

    return rangeTypeNames;
}

QVector<Profile::RangeType> Profile::getRangeTypes() const
{
    return _rangeTypes;
}