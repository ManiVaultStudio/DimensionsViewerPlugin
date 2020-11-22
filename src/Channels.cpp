#include "Channels.h"
#include "Channel.h"
#include "Profile.h"
#include "ConfigurationsModel.h"
#include "StandardItems.h"
#include "Visitor.h"

#include <QDebug>

const QMap<QString, Channels::Child> Channels::children = {
    { "Dataset", Channels::Child::Dataset },
    { "Subset1", Channels::Child::Subset1 },
    { "Subset2", Channels::Child::Subset2 },
    { "Compare", Channels::Child::Differential }
};

Channels::Channels(Item* parent, const QString& datasetName, const QString& dataName) :
    Item(parent, "Channels", "Channels")
{
    _flags.setFlag(Qt::ItemIsEditable);
    _flags.setFlag(Qt::ItemIsEnabled);

    _children << new Channel(this, 0, children.key(Channels::Child::Dataset), true, false, datasetName);
    _children << new Channel(this, 1, children.key(Channels::Child::Subset1), false, true, "");
    _children << new Channel(this, 2, children.key(Channels::Child::Subset2), false, true, "");
    _children << new Channel(this, 3, children.key(Channels::Child::Differential), false, false, "");
}

void Channels::initialize()
{
    const auto synchronize = [this](const QStringList& channelNames, const QStringList& itemNames) {
        for (auto channelName : channelNames) {
            if (!getChild(QString("%1/Linked").arg(channelName))->getData(Column::Value, Qt::EditRole).toBool())
                continue;

            for (auto itemName : itemNames) {
                const auto sourcePath = QString("Dataset/Profile/%2").arg(itemName);
                const auto targetPath = QString("%1/Profile/%2").arg(channelName, itemName);

                getChild(targetPath)->copy(getChild(sourcePath));
            }
        }
    };

    QObject::connect(getChild("Dataset/Profile/ProfileTypes"), &tree::String::dataChanged, [this, synchronize](const QModelIndex& modelIndex) {
        synchronize({ "Subset1", "Subset2" }, { "ProfileTypes" });
    });

    QObject::connect(getChild("Dataset/Profile/ProfileType"), &tree::String::dataChanged, [this, synchronize](const QModelIndex& modelIndex) {
        synchronize({ "Subset1", "Subset2" }, { "ProfileType" });
    });

    QObject::connect(getChild("Dataset/Profile/RangeTypes"), &tree::String::dataChanged, [this, synchronize](const QModelIndex& modelIndex) {
        synchronize({ "Subset1", "Subset2" }, { "RangeTypes" });
    });

    QObject::connect(getChild("Dataset/Profile/RangeType"), &tree::String::dataChanged, [this, synchronize](const QModelIndex& modelIndex) {
        synchronize({ "Subset1", "Subset2" }, { "RangeType" });
    });

    synchronize({ "Subset1", "Subset2" }, { "ProfileTypes", "ProfileType", "RangeTypes", "RangeType" });
}

void Channels::accept(tree::Visitor* visitor) const
{
    visitor->visitTreeItem(this);
}

QVector<Channel*> Channels::getFiltered(const Profile::ProfileTypes& profileTypes, bool* enabled /*= nullptr*/) const
{
    QVector<Channel*> channels;

    /*for (auto channel : _channels) {
        const auto profileType = channel->getProfile()->_profileType;

        if (!profileTypes.isEmpty() && !profileTypes.contains(profileType))
            continue;

        if (enabled != nullptr && channel->isEnabled() != *enabled)
            continue;

        channels << channel;
    }*/

    return channels;
}