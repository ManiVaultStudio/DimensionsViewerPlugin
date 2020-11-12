#include "Channels.h"
#include "Channel.h"
#include "Configuration.h"

#include <QDebug>

const QMap<QString, Channels::Column> Channels::columns = {
    { "Type", Channels::Column::Type }
};

const QMap<QString, Channels::Row> Channels::rows = {
    { "Channel 1", Channels::Row::Channel1 },
    { "Channel 2", Channels::Row::Channel2 },
    { "Channel 3", Channels::Row::Channel3 },
    { "Differential", Channels::Row::Differential }
};

Channels::Channels(ModelItem* parent, const QString& datasetName, const QString& dataName) :
    ModelItem("Channels", parent),
    _channels({
        new Channel(this, 0, "Dataset", true, datasetName, Profile::ProfileType::Mean, Qt::black, 0.25f),
        new Channel(this, 1, "Subset 1", false, "", Profile::ProfileType::Mean, QColor(249, 149, 0), 0.25f),
        new Channel(this, 2, "Subset 2", false, "", Profile::ProfileType::Mean, QColor(0, 112, 249), 0.25f),
        new Channel(this, 3, "Compare", false, "", Profile::ProfileType::Differential, QColor(150, 70, 20), 0.25f)
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

std::int32_t Channels::getNoChannelsEnabled() const
{
    auto noChannelsEnabled = 0;

    for (auto channel : _channels)
        if (channel->_enabled)
            noChannelsEnabled++;

    return noChannelsEnabled;
}