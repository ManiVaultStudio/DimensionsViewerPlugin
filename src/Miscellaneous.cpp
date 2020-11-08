#include "Miscellaneous.h"

#include <QDebug>

const QMap<QString, Miscellaneous::Column> Miscellaneous::columns = {
    { "Name", Miscellaneous::Column::Name },
    { "Show dimension names", Miscellaneous::Column::ShowDimensionNames }
};

const QMap<Miscellaneous::Column, std::function<QVariant(Miscellaneous* miscellaneous)>> Miscellaneous::getEditRoles = {
    { Miscellaneous::Column::Name, [](Miscellaneous* miscellaneous) {
        return miscellaneous->_name;
    }},
    { Miscellaneous::Column::ShowDimensionNames, [](Miscellaneous* miscellaneous) {
        return miscellaneous->_showDimensionNames;
    }}
};

const QMap<Miscellaneous::Column, std::function<QVariant(Miscellaneous* miscellaneous)>> Miscellaneous::getDisplayRoles = {
    { Miscellaneous::Column::Name, [](Miscellaneous* miscellaneous) {
        return getEditRoles[Miscellaneous::Column::Name](miscellaneous).toString();
    }},
    { Miscellaneous::Column::ShowDimensionNames, [](Miscellaneous* miscellaneous) {
        return getEditRoles[Miscellaneous::Column::ShowDimensionNames](miscellaneous).toBool() ? "on" : "off";
    }}
};

const QMap<Miscellaneous::Column, std::function<QModelIndexList(Miscellaneous* miscellaneous, const QModelIndex& index, const QVariant& value)>> Miscellaneous::setEditRoles = {
    { Miscellaneous::Column::ShowDimensionNames, [](Miscellaneous* miscellaneous, const QModelIndex& index, const QVariant& value) {
        miscellaneous->_showDimensionNames = value.toBool();

        return QModelIndexList();
    }}
};

Miscellaneous::Miscellaneous(ModelItem* parent) :
    ModelItem("Miscellaneous", parent),
    _showDimensionNames(false)
{
}

int Miscellaneous::columnCount() const 
{
    return ModelItem::maxNoColumns;
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

QVariant Miscellaneous::getData(const QModelIndex& index, const int& role) const
{
    const auto column = index.column();

    switch (role)
    {
        case Qt::EditRole:
        {
            if (getEditRoles.contains(static_cast<Column>(column)))
                return getEditRoles[static_cast<Column>(column)](const_cast<Miscellaneous*>(this));

            break;
        }

        case Qt::DisplayRole:
        {
            if (getDisplayRoles.contains(static_cast<Column>(column)))
                return getDisplayRoles[static_cast<Column>(column)](const_cast<Miscellaneous*>(this));

            break;
        }

        default:
            break;
    }

    return QVariant();
}

QModelIndexList Miscellaneous::setData(const QModelIndex& index, const QVariant& value, const int& role)
{
    const auto column = static_cast<Column>(index.column());

    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole:
        {
            if (setEditRoles.contains(column))
                affectedIndices << setEditRoles[column](const_cast<Miscellaneous*>(this), index, value);

            break;
        }

        default:
            break;
    }

    return affectedIndices;
}