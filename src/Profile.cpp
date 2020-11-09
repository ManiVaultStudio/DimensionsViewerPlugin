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

const QMap<Profile::Column, std::function<QVariant(Profile* profile)>> Profile::getEditRoles = {
    { Profile::Column::Name, [](Profile* profile) {
        return profile->_name;
    }},
    { Profile::Column::ProfileTypes, [](Profile* profile) {
        return profile->getProfileTypeNames();
    }},
    { Profile::Column::ProfileType, [](Profile* profile) {
        return to_ul(profile->getProfileType());
    }},
    { Profile::Column::RangeTypes, [](Profile* profile) {
        return profile->getRangeTypeNames();
    }},
    { Profile::Column::RangeType, [](Profile* profile) {
        return to_ul(profile->getRangeType());
    }}
};

const QMap<Profile::Column, std::function<QVariant(Profile* profile)>> Profile::getDisplayRoles = {
    { Profile::Column::Name, [](Profile* profile) {
        return getEditRoles[Profile::Column::Name](profile);
    }},
    { Profile::Column::ProfileTypes, [](Profile* profile) {
        return getEditRoles[Profile::Column::ProfileTypes](profile).toStringList().join(", ");
    }},
    { Profile::Column::ProfileType, [](Profile* profile) {
        return QString::number(getEditRoles[Profile::Column::ProfileType](profile).toInt());
    }},
    { Profile::Column::RangeTypes, [](Profile* profile) {
        return getEditRoles[Profile::Column::RangeTypes](profile).toStringList().join(", ");
    }},
    { Profile::Column::RangeType, [](Profile* profile) {
        return QString::number(getEditRoles[Profile::Column::RangeType](profile).toInt());
    }}
};

const QMap<Profile::Column, std::function<QModelIndexList(Profile* profile, const QModelIndex& index, const QVariant& value)>> Profile::setEditRoles = {
    { Profile::Column::ProfileType, [](Profile* profile, const QModelIndex& index, const QVariant& value) {
        profile->setProfileType(static_cast<Profile::ProfileType>(value.toInt()));
        return QModelIndexList();
    }},
    { Profile::Column::RangeType, [](Profile* profile, const QModelIndex& index, const QVariant& value) {
        profile->setRangeType(static_cast<Profile::RangeType>(value.toInt()));
        return QModelIndexList();
    }}
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

QVariant Profile::getData(const QModelIndex& index, const int& role) const
{
    const auto column = index.column();

    switch (role)
    {
        case Qt::EditRole:
        {
            if (getEditRoles.contains(static_cast<Column>(column)))
                return getEditRoles[static_cast<Column>(column)](const_cast<Profile*>(this));

            break;
        }

        case Qt::DisplayRole:
        {
            if (getDisplayRoles.contains(static_cast<Column>(column)))
                return getDisplayRoles[static_cast<Column>(column)](const_cast<Profile*>(this));

            break;
        }

        default:
            break;
    }

    return QVariant();
}

QModelIndexList Profile::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    const auto column = static_cast<Column>(index.column());

    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole:
        {
            if (setEditRoles.contains(column))
                affectedIndices << setEditRoles[column](const_cast<Profile*>(this), index, value);

            break;
        }

        default:
            break;
    }

    return affectedIndices;
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

bool Profile::canDisplay() const
{
    if (_profileType != ProfileType::None && _profileType != ProfileType::Differential)
        return true;
    
    if (_rangeType != RangeType::None)
        return true;

    return false;
}