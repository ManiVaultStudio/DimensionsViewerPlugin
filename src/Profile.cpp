#include "Profile.h"
#include "Channel.h"
#include "StandardItems.h"
#include "Visitor.h"

#include <QDebug>

const QMap<QString, Profile::Child> Profile::children = {
    { "Profile type", Profile::Child::ProfileType },
    { "Range type", Profile::Child::RangeType }
};

const QMap<QString, Profile::ProfileType> Profile::profileTypes = {
    { "Mean", Profile::ProfileType::Mean },
    { "Median", Profile::ProfileType::Median },
    { "Differential", Profile::ProfileType::Differential }
};

const QMap<QString, Profile::RangeType> Profile::rangeTypes = {
    { "1 StdDev", Profile::RangeType::StandardDeviation1 },
    { "2 StdDev", Profile::RangeType::StandardDeviation2 },
    { "3 StdDev", Profile::RangeType::StandardDeviation3 },
    { "Min/Max", Profile::RangeType::MinMax },
    { "5/95 perc.", Profile::RangeType::Percentile5 },
    { "10/90 perc.", Profile::RangeType::Percentile10 }
};

Profile::Profile(Item* parent /*= nullptr*/, const ProfileType& profileType /*= ProfileType::Mean*/) :
    Item(parent, "Profile", "Profile"),
    _sourceProfile(nullptr)
{
    _flags.setFlag(Qt::ItemIsEditable);
    _flags.setFlag(Qt::ItemIsEnabled);

    const auto profileTypeNames = getProfileTypeNames();

    _children << new tree::StringList(this, "ProfileTypes", profileTypeNames);
    _children << new tree::String(this, "ProfileType", profileTypeNames.first());
    _children << new tree::StringList(this, "RangeTypes");
    _children << new tree::String(this, "RangeType");
}

void Profile::initialize()
{
    const auto update = [this](const QModelIndex& modelIndex) {
        if (static_cast<Column>(modelIndex.column()) != Column::Value)
            return;

        if (_parent->getChild(to_ul(Channel::Child::Linked))->getData(Column::Value, Qt::EditRole).toBool())
            return;

        const auto profileTypeName  = _children[to_ul(Child::ProfileType)]->getData(Column::Value, Qt::DisplayRole).toString();
        const auto rangeTypeName    = _children[to_ul(Child::RangeType)]->getData(Column::Value, Qt::DisplayRole).toString();
        const auto rangeTypeNames   = Profile::getRangeTypeNames(getProfileTypeEnum(profileTypeName));

        _children[to_ul(Child::RangeTypes)]->setData(Column::Value, rangeTypeNames, Qt::EditRole);

        if (rangeTypeNames.isEmpty())
            return;

        if (rangeTypeNames.contains(rangeTypeName))
            _children[to_ul(Child::RangeTypes)]->setData(Column::Value, rangeTypeName, Qt::DisplayPropertyRole);
        else
            _children[to_ul(Child::RangeTypes)]->setData(Column::Value, rangeTypeNames.first(), Qt::DisplayRole);
    };

    QObject::connect(_children[to_ul(Child::ProfileType)], &tree::String::dataChanged, update);

    update(getSiblingAtColumn(to_ul(Column::Value)));
}

void Profile::accept(tree::Visitor* visitor) const
{
    visitor->visitTreeItem(this);
}

void Profile::setSourceProfile(Profile* sourceProfile)
{
    _sourceProfile = sourceProfile;

    if (_sourceProfile == nullptr) {
        //QObject::connect(sourceProfile->getChild(to_ul(Child::ProfileType)), &tree::String::dataChanged, this);
        return;
    }

    QObject::connect(sourceProfile->getChild(to_ul(Child::ProfileType)), &tree::String::dataChanged, [this](const QModelIndex& modelIndex) {
        if (_parent->getChild(to_ul(Channel::Child::Linked))->getData(Column::Value, Qt::EditRole).toBool())
            return;

        const auto profileTypeName = _sourceProfile->getChild(to_ul(Profile::Child::ProfileType))->getData(Column::Value, Qt::DisplayRole).toString();

        _children[to_ul(Child::ProfileType)]->setData(Column::Value, profileTypeName, Qt::DisplayRole);
    });
}