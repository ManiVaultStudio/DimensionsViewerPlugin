#include "Channels.h"
#include "Channel.h"
#include "Configuration.h"

#include <QDebug>

const QMap<QString, Channels::Column> Channels::columns = {
    { "Type", Channels::Column::Type }
};

const QMap<QString, Channels::Row> Channels::rows = {
    { "Dataset", Channels::Row::Dataset },
    { "Subset 1", Channels::Row::Subset1 },
    { "Subset 2", Channels::Row::Subset2 },
    { "Compare", Channels::Row::Differential }
};

Channels::Channels(ModelItem* parent, const QString& datasetName, const QString& dataName) :
    ModelItem("Channels", parent),
    _channels({
        new Channel(this, 0, getRowTypeName(Channels::Row::Dataset), true, datasetName, Profile::ProfileType::Mean, Qt::black, 0.25f),
        new Channel(this, 1, getRowTypeName(Channels::Row::Subset1), false, "", Profile::ProfileType::Mean, QColor(249, 149, 0), 0.25f),
        new Channel(this, 2, getRowTypeName(Channels::Row::Subset2), false, "", Profile::ProfileType::Mean, QColor(0, 112, 249), 0.25f),
        new Channel(this, 3, getRowTypeName(Channels::Row::Differential), false, "", Profile::ProfileType::Differential, QColor(255, 10, 40), 0.25f)
    })
{
}

int Channels::columnCount() const
{
    return to_ul(Column::_Count);
}

Qt::ItemFlags Channels::getFlags(const QModelIndex& index) const
{
    return Qt::NoItemFlags;
}

QVariant Channels::getData(const std::int32_t& column, const std::int32_t& role) const
{
    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(column))
            {
                case Channels::Column::Type:
                    return _type;

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole: {

            switch (static_cast<Column>(column))
            {
                case Channels::Column::Type:
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

QModelIndexList Channels::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(index.column()))
            {
                case Channels::Column::Type:
                    break;

                default:
                    break;
            }

            break;
        }

        default:
            break;
    }

    return affectedIndices;
}

ModelItem* Channels::getChild(const int& index) const
{
    try
    {
        return _channels[index];
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

int Channels::getChildIndex(ModelItem* child) const
{
    const auto channel = dynamic_cast<Channel*>(child);

    if (channel == nullptr)
        return 0;

    return _channels.indexOf(channel);
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

        if (enabled != nullptr && channel->getData(to_ul(Channel::Column::Enabled), Qt::EditRole).toBool() != *enabled)
            continue;

        channels << channel;
    }

    return channels;
}