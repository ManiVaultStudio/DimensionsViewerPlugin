#include "Channels.h"
#include "Channel.h"
#include "Configuration.h"
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

const Configuration* Channels::getConfiguration() const
{
    return dynamic_cast<Configuration*>(parent());
}

QVector<Channel*> Channels::getFiltered(const Profile::ProfileTypes& profileTypes, bool* enabled /*= nullptr*/) const
{
    QVector<Channel*> channels;

    for (auto channel : _channels) {
        const auto profileType = channel->getProfile().getProfileType();

        if (!profileTypes.isEmpty() && !profileTypes.contains(profileType))
            continue;

        if (enabled != nullptr && channel->getData(to_ul(TreeItem::Column::Enabled), Qt::EditRole).toBool() != *enabled)
            continue;

        channels << channel;
    }

    return channels;
}