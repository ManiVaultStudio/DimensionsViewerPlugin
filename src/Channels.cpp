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
    _flags.setFlag(Qt::ItemIsEnabled);

    _children << new Channel(this, 0, children.key(Channels::Child::Dataset), true, false, datasetName);
    _children << new Channel(this, 1, children.key(Channels::Child::Subset1), false, true, "");
    _children << new Channel(this, 2, children.key(Channels::Child::Subset2), false, true, "");
    _children << new Channel(this, 3, children.key(Channels::Child::Differential), false, false, "");
}

void Channels::initialize()
{
    /*
    const auto synchronize = [this](const QStringList& channelNames = { "Subset1", "Subset2" }, const QStringList& itemNames = { "ProfileTypes", "ProfileType", "RangeTypes", "RangeType" }) {
        for (auto channelName : channelNames) {
            if (!getChild(QString("%1/Linked").arg(channelName))->getValue().toBool())
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

    QObject::connect(getChild("Subset1/Linked"), &tree::Boolean::dataChanged, [this, synchronize](const QModelIndex& modelIndex) {
        synchronize();
    });
    
    synchronize();
    */

    QObject::connect(getChild("../Subsets"), &tree::StringList::dataChanged, [this](const QModelIndex& modelIndex) {
        getChild("Subset1/DatasetNames")->copy(getChild("../Subsets"));
        getChild("Subset2/DatasetNames")->copy(getChild("../Subsets"));
    });
    
    QObject::connect(getChild("../../../DatasetNames"), &tree::StringList::dataChanged, [this](const QModelIndex& modelIndex) {
        getChild("Dataset/DatasetNames")->copy(getChild("../../../DatasetNames"));
    });

    QObject::connect(getChild("Subset1/DatasetNames"), &tree::StringList::dataChanged, [this](const QModelIndex& modelIndex) {
        getChild("Subset1/Enabled")->setFlag(Qt::ItemIsEnabled, getChild("Subset1/DatasetNames")->getValue().toStringList().count() >= 1);
    });

    QObject::connect(getChild("Subset2/DatasetNames"), &tree::StringList::dataChanged, [this](const QModelIndex& modelIndex) {
        getChild("Subset2/Enabled")->setFlag(Qt::ItemIsEnabled, getChild("Subset2/DatasetNames")->getValue().toStringList().count() >= 2);
    });
    
    getChild("Subset1/DatasetNames")->setDataChanged();
    getChild("Subset2/DatasetNames")->setDataChanged();

    //getChild("Dataset/Linked")->unsetFlag(...);

    /*
    QObject::connect(getChild("Dataset/DatasetNames"), &tree::StringList::dataChanged, [this](const QModelIndex& modelIndex) {
        const auto datasetNames = getChild("Dataset/DatasetNames")->getValue().toStringList();

        getChild("Dataset/DatasetName")->setFlag(Qt::ItemIsEnabled, datasetNames.count() >= 2);
    });

    

    QObject::connect(getChild("Dataset/DatasetName"), &tree::StringList::dataChanged, [this](const QModelIndex& modelIndex) {
        const auto datasetName = getChild("Dataset/DatasetName")->getValue().toString();

        if (datasetName.isEmpty())
            return;

        auto configurationsModel = dynamic_cast<ConfigurationsModel*>(model);

        Q_ASSERT(configurationsModel != nullptr);

        configurationsModel->selectRow(datasetName);
    });

    const auto onChannelEnabled = [this](const QString& channelName) {
        getChild(channelName)->setFlag(Qt::ItemIsEnabled, getChild(QString("%1/Enabled").arg(channelName))->getValue().toBool());
    };

    QObject::connect(getChild("Dataset/Enabled"), &tree::String::dataChanged, [this, onChannelEnabled](const QModelIndex& modelIndex) {
        onChannelEnabled("Dataset");
    });*/
    //QObject::connect(getChild("Subset1/Enabled"), &tree::String::dataChanged, update);
    //QObject::connect(getChild("Subset1/DatasetNames"), &tree::String::dataChanged, update);
    //QObject::connect(getChild("Subset2/Enabled"), &tree::String::dataChanged, update);
    //QObject::connect(getChild("Subset2/DatasetNames"), &tree::String::dataChanged, update);


    //QObject::connect(getChild("Differential/Enabled"), &tree::String::dataChanged, update);
    //QObject::connect(getChild("Differential/DatasetNames"), &tree::String::dataChanged, update);
}

void Channels::accept(tree::Visitor* visitor) const
{
    Q_ASSERT(visitor != nullptr);

    visitor->visitTreeItem(this);
}