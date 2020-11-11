#include "Global.h"
#include "Channels.h"
#include "Channel.h"
#include "ConfigurationsModel.h"

#include <QDebug>

const QMap<QString, Global::Column> Global::columns = {
    { "Name", Global::Column::Name },
    { "Enabled", Global::Column::Enabled }
};

Global::Global(ModelItem* parent) :
    ModelItem("Global", parent),
    _enabled(true),
    _profile(this, Profile::ProfileType::Mean)
{
}

int Global::columnCount() const 
{
    return to_ul(Column::Count);
}

Qt::ItemFlags Global::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = Qt::ItemIsEditable;

    const auto column = static_cast<Column>(index.column());

    switch (column)
    {
        case Column::Name: {
            flags |= Qt::ItemIsEnabled;
            break;
        }

        case Column::Enabled: {
            flags |= Qt::ItemIsEnabled;
            break;
        }

        default:
            break;
    }

    return flags;
}

QVariant Global::getData(const std::int32_t& column, const std::int32_t& role) const
{
    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(column))
            {
                case Global::Column::Name:
                    return _name;

                case Global::Column::Enabled:
                    return _enabled;

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole: {

            switch (static_cast<Column>(column))
            {
                case Global::Column::Name:
                    return getData(column, Qt::EditRole);

                case Global::Column::Enabled:
                    return getData(column, Qt::EditRole).toBool() ? "on" : "off";

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

QModelIndexList Global::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(index.column()))
            {
                case Global::Column::Enabled:
                {
                    _enabled = value.toBool();

                    break;
                }
                
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

ModelItem* Global::getChild(const int& index) const
{
    switch (static_cast<Row>(index))
    {
        case Row::Profile:
            return const_cast<Profile*>(&_profile);

        default:
            break;
    }
    return nullptr;
}

int Global::getChildCount() const
{
    return to_ul(Row::Count);
}

int Global::getChildIndex(ModelItem* child) const
{
    if (dynamic_cast<Profile*>(child))
        return to_ul(Row::Profile);

    return 0;
}