#include "Channels.h"
#include "Channel.h"
#include "Configuration.h"

#include <QDebug>

const QMap<QString, Channels::Column> Channels::columns = {
    { "Name", Channels::Column::Name }
};

Channels::Channels(ModelItem* parent, const QString& datasetName, const QString& dataName) :
    ModelItem("Channels", parent),
    _channels({
        new Channel(this, 0, "Dataset", true, datasetName, dataName, Qt::black, 0.25f),
        new Channel(this, 1, "Subset 1", false, "", dataName, QColor(249, 149, 0), 0.25f),
        new Channel(this, 2, "Subset 2", false, "", dataName, QColor(0, 112, 249), 0.25f)
    })
{
}

int Channels::columnCount() const
{
    return to_ul(Column::Count);
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
                case Channels::Column::Name:
                    return _name;

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole: {

            switch (static_cast<Column>(column))
            {
                case Channels::Column::Name:
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

ModelItem::AffectedColumns Channels::setData(const std::int32_t& column, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    AffectedColumns affectedColunns{ column };

    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(column))
            {
                case Channels::Column::Name:
                    break;

                default:
                    break;
            }

            break;
        }

        default:
            break;
    }

    return affectedColunns;
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