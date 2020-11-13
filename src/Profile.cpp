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

Profile::Profile(const ProfileType& profileType) :
    _locked(profileType == ProfileType::Differential),
    _profileTypes(),
    _profileType(profileType),
    _rangeType(RangeType::MinMax),
    _rangeTypes()
{
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