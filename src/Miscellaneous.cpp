#include "Miscellaneous.h"

#include <QDebug>

const QMap<QString, Miscellaneous::Column> Miscellaneous::columns = {
    { "Name", Miscellaneous::Column::Name },
    { "Show dimension names", Miscellaneous::Column::ShowDimensionNames }
};

Miscellaneous::Miscellaneous(ModelItem* parent) :
    ModelItem("Miscellaneous", parent),
    _showDimensionNames(false)
{
}

int Miscellaneous::columnCount() const 
{
    return to_ul(Column::Count);
}

Qt::ItemFlags Miscellaneous::getFlags(const QModelIndex& index) const
{
    Qt::ItemFlags flags;

    const auto column = static_cast<Column>(index.column());

    switch (column)
    {
        case Column::Name: {
            flags |= Qt::ItemIsEnabled;

            break;
        }

        case Column::ShowDimensionNames: {
            if (_showDimensionNames)
                flags |= Qt::ItemIsEnabled;

            break;
        }

        default:
            break;
    }

    return flags;
}

QVariant Miscellaneous::getData(const std::int32_t& column, const std::int32_t& role) const
{
    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(column))
            {
                case Miscellaneous::Column::Name:
                    return _name;

                case Miscellaneous::Column::ShowDimensionNames:
                    return _showDimensionNames;

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole: {

            switch (static_cast<Column>(column))
            {
                case Miscellaneous::Column::Name:
                    return getData(column, Qt::EditRole);

                case Miscellaneous::Column::ShowDimensionNames:
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

QModelIndexList Miscellaneous::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(index.column()))
            {
                case Miscellaneous::Column::ShowDimensionNames: {
                    _showDimensionNames = value.toBool();
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