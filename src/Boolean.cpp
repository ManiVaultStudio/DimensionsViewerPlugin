#include "Boolean.h"
#include "Visitor.h"

namespace tree {

Boolean::Boolean(Item* parent, const QString& name, const bool& value /*= true*/) :
    Item(parent, "Boolean", name),
    _value(value)
{
}

QVariant Boolean::getData(const QModelIndex& index, const int& role) const
{
    if (static_cast<Column>(index.column()) != Column::Value)
        return Item::getData(index, role);
    
    switch (role)
    {
        case Qt::EditRole:
            return _value;

        case Qt::DisplayRole:
            return _value ? "on" : "off";

        case Qt::ToolTipRole:
            return QString("%1: %2").arg(_name, getData(index, Qt::DisplayRole).toString());

        default:
            break;
    }

    return QVariant();
}

void Boolean::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    if (static_cast<Column>(index.column()) != Column::Value) {
        Item::setData(index, value, role);
        return;
    }

    switch (role)
    {
        case Qt::EditRole:
            _value = value.toBool();
            break;

        case Qt::DisplayRole:
            break;

        default:
            break;
    }
}

void Boolean::accept(Visitor* visitor) const
{
    visitor->visitTreeItem(this);
}

}