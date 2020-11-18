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

Channels::Channels(TreeItem* parent, const QString& datasetName, const QString& dataName) :
    TreeItem("Channels", "Channels", parent),
    _channels()
{
    setNumColumns(to_ul(Column::_Count));

    _channels[Row::Dataset]         = new Channel(this, 0, getRowTypeName(Channels::Row::Dataset), true, false, datasetName, Profile::ProfileType::Mean, Qt::black, 1.0f);
    _channels[Row::Subset1]         = new Channel(this, 1, getRowTypeName(Channels::Row::Subset1), false, true, "", Profile::ProfileType::Mean, QColor(249, 149, 0), 1.0f);
    _channels[Row::Subset2]         = new Channel(this, 2, getRowTypeName(Channels::Row::Subset2), false, true, "", Profile::ProfileType::Mean, QColor(0, 112, 249), 1.0f);
    _channels[Row::Differential]    = new Channel(this, 3, getRowTypeName(Channels::Row::Differential), false, false, "", Profile::ProfileType::Differential, QColor(255, 20, 20), 0.5f);

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
                /*if (modelIndex.data(Qt::EditRole).toBool()) {
                    getModel()->setData(_channels.value(Row::Subset1)->getProfile(), to_ul(Profile::Column::ProfileTypes), modelIndex.data(Qt::EditRole), Qt::EditRole);
                    getModel()->setData(_channels.value(Row::Subset1)->getProfile(), to_ul(Profile::Column::ProfileType), modelIndex.data(Qt::EditRole), Qt::EditRole);
                }*/
                    
                break;
            }

            default:
                break;
        }
    });
}

Qt::ItemFlags Channels::getFlags(const QModelIndex& index) const
{
    return TreeItem::getFlags(index);
}

QVariant Channels::getData(const std::int32_t& column, const std::int32_t& role) const
{
    return TreeItem::getData(column, role);
}

QModelIndexList Channels::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    return TreeItem::setData(index, value, role);
}

TreeItem* Channels::getChild(const int& index) const
{
    try
    {
        return _channels[static_cast<Row>(index)];
    }
    catch (std::exception exception)
    {
        return nullptr;
    }
}

int Channels::getChildCount() const
{
    return _channels.size();
}

int Channels::getChildIndex(TreeItem* child) const
{
    const auto channel = dynamic_cast<Channel*>(child);

    if (channel == nullptr)
        return 0;

    return _channels.values().indexOf(channel);
}

void Channels::accept(Visitor* visitor) const
{
    visitor->visitChannels(this);
}

QVector<Channel*> Channels::getFiltered(const Profile::ProfileTypes& profileTypes, bool* enabled /*= nullptr*/) const
{
    QVector<Channel*> channels;

    /*for (auto channel : _channels) {
        const auto profileType = channel->getProfile()->getProfileType();

        if (!profileTypes.isEmpty() && !profileTypes.contains(profileType))
            continue;

        if (enabled != nullptr && channel->getData(to_ul(TreeItem::Column::Enabled), Qt::EditRole).toBool() != *enabled)
            continue;

        channels << channel;
    }*/

    return channels;
}