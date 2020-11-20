#include "Profile.h"
#include "StandardItems.h"
#include "Visitor.h"

#include <QDebug>

const QMap<QString, Profile::Child> Profile::children = {
    { "Profile type", Profile::Child::ProfileType },
    { "Range type", Profile::Child::RangeType }
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

    _children << new tree::StringList(this, "Profile types", QStringList(profileTypes.keys()));
    _children << new tree::String(this, "Profile type", profileTypes.keys().first());
    _children << new tree::StringList(this, "Range types");
    _children << new tree::String(this, "Range type");

    QObject::connect(_children[to_ul(Child::ProfileType)], &tree::Boolean::dataChanged, [this](const QModelIndex& modelIndex) {
        const auto profileType = _children[to_ul(Child::ProfileType)]->getData(Column::Value, Qt::DisplayRole).toString();
        qDebug() << profileType;
        _children[to_ul(Child::RangeTypes)]->setData(Column::Value, getRangeTypes(getProfileTypeEnum(profileType)), Qt::EditRole);
    });
}

void Profile::accept(tree::Visitor* visitor) const
{
    visitor->visitTreeItem(this);
}

QStringList Profile::getRangeTypes(const ProfileType& profileType)
{
    QStringList rangeTypes;

    rangeTypes.clear();

    switch (profileType)
    {
        case ProfileType::None:
        case ProfileType::Differential:
        {
            //_rangeType = RangeType::None;

            break;
        }

        case ProfileType::Mean:
        {
            rangeTypes << getRangeTypeName(RangeType::None);
            rangeTypes << getRangeTypeName(RangeType::StandardDeviation1);
            rangeTypes << getRangeTypeName(RangeType::StandardDeviation2);
            rangeTypes << getRangeTypeName(RangeType::StandardDeviation3);
            rangeTypes << getRangeTypeName(RangeType::MinMax);

            //if (!rangeTypes.contains(_rangeType))
            //    _rangeType = RangeType::StandardDeviation1;

            break;
        }

        case ProfileType::Median:
        {
            rangeTypes << getRangeTypeName(RangeType::Percentile5);
            rangeTypes << getRangeTypeName(RangeType::Percentile10);

            //if (!rangeTypes.contains(_rangeType))
            //    _rangeType = RangeType::Percentile5;

            break;
        }

        default:
            break;
    }

    return rangeTypes;
}