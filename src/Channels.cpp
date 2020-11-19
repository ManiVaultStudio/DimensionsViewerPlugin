#include "Channels.h"
#include "Channel.h"
#include "Profile.h"
#include "ConfigurationsModel.h"
#include "Visitor.h"

#include <QDebug>

const QMap<QString, Channels::Row> Channels::rows = {
    { "Dataset", Channels::Row::Dataset },
    { "Subset 1", Channels::Row::Subset1 },
    { "Subset 2", Channels::Row::Subset2 },
    { "Compare", Channels::Row::Differential }
};

Channels::Channels(Item* parent, const QString& datasetName, const QString& dataName) :
    Item(parent, "Channels", "Channels")
{
    _children << new ChannelItem(this, 0, getRowTypeName(Channels::Row::Dataset), true, false, datasetName);
    _children << new ChannelItem(this, 1, getRowTypeName(Channels::Row::Subset1), false, true, "");
    _children << new ChannelItem(this, 2, getRowTypeName(Channels::Row::Subset2), false, true, "");
    _children << new ChannelItem(this, 3, getRowTypeName(Channels::Row::Differential), false, false, "");

    /*
    QObject::connect(_channels[Row::Dataset], &Profile::dataChanged, [this](const QModelIndex& modelIndex) {
        if (static_cast<Channel::Column>(modelIndex.column()) == Channel::Column::Enabled) {
            const auto enabledIndex = _channels[Row::Differential]->getSiblingAtColumn(to_ul(Channel::Column::Enabled));
            emit getModel()->dataChanged(enabledIndex, enabledIndex);
        }
    });

    QObject::connect(_channels[Row::Subset1], &Profile::dataChanged, [this](const QModelIndex& modelIndex) {
        if (static_cast<Channel::Column>(modelIndex.column()) == Channel::Column::Enabled) {
            const auto enabledIndex = _channels[Row::Differential]->getSiblingAtColumn(to_ul(Channel::Column::Enabled));
            emit getModel()->dataChanged(enabledIndex, enabledIndex);
        }
    });

    QObject::connect(_channels[Row::Subset2], &Profile::dataChanged, [this](const QModelIndex& modelIndex) {
        if (static_cast<Channel::Column>(modelIndex.column()) == Channel::Column::Enabled) {
            const auto enabledIndex = _channels[Row::Differential]->getSiblingAtColumn(to_ul(Channel::Column::Enabled));
            emit getModel()->dataChanged(enabledIndex, enabledIndex);
        }
    });

    QObject::connect(_channels[Row::Dataset]->getProfile(), &Profile::dataChanged, [this](const QModelIndex& modelIndex) {
        switch (static_cast<Profile::Column>(modelIndex.column()))
        {
            case Profile::Column::ProfileTypes:
            case Profile::Column::ProfileType:
            case Profile::Column::RangeTypes:
            case Profile::Column::RangeType:
            {
                auto dataset = _channels.value(Row::Dataset);
                auto subset1 = _channels.value(Row::Subset1);
                auto subset2 = _channels.value(Row::Subset2);

                if (subset1->getData(Channel::Column::Linked, Qt::EditRole).toBool())
                    subset1->getProfile()->setProfile(dataset->getProfile());

                if (subset2->getData(Channel::Column::Linked, Qt::EditRole).toBool())
                    subset2->getProfile()->setProfile(dataset->getProfile());

                break;
            }

            default:
                break;
        }
    });

    QObject::connect(_channels.value(Row::Subset1), &Channel::dataChanged, [this](const QModelIndex& modelIndex) {
        switch (static_cast<Channel::Column>(modelIndex.column()))
        {
            case Channel::Column::Linked:
            {
                if (modelIndex.data(Qt::EditRole).toBool()) {
                    getModel()->setData(_channels.value(Row::Subset1)->getProfile(), to_ul(Profile::Column::ProfileTypes), modelIndex.data(Qt::EditRole), Qt::EditRole);
                    getModel()->setData(_channels.value(Row::Subset1)->getProfile(), to_ul(Profile::Column::ProfileType), modelIndex.data(Qt::EditRole), Qt::EditRole);
                }
                    
                break;
            }

            default:
                break;
        }
    });
    */
}

void Channels::accept(tree::Visitor* visitor) const
{
    visitor->visitTreeItem(this);
}

QVector<ChannelItem*> Channels::getFiltered(const Profile::ProfileTypes& profileTypes, bool* enabled /*= nullptr*/) const
{
    QVector<ChannelItem*> channels;

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