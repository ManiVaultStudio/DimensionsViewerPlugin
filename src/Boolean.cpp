#include "Boolean.h"
#include "Visitor.h"

namespace tree {

Boolean::Boolean(Item* parent, const QString& name, const bool& value /*= true*/) :
    StandardItem(parent, "Boolean", name),
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

bool Boolean::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    if (static_cast<Column>(index.column()) != Column::Value)
        return Item::setData(index, value, role);

    switch (role)
    {
        case Qt::EditRole:
        {
            if (value.toBool() != _value) {
                _value = value.toBool();
                _modified++;

                return true;
            }

            break;
        }

        case Qt::DisplayRole:
            break;

        default:
            break;
    }

    return false;
}

void Boolean::accept(Visitor* visitor) const
{
    visitor->visitTreeItem(this);
}

}