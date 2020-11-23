#include "String.h"
#include "Visitor.h"

namespace tree {

String::String(Item* parent, const QString& name, const QString& value /*= ""*/) :
    StandardItem(parent, "String", name),
    _value(value)
{
}

QVariant String::getData(const QModelIndex& index, const int& role) const
{
    if (static_cast<Column>(index.column()) != Column::Value)
        return Item::getData(index, role);
    
    switch (role)
    {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return _value;

        case Qt::ToolTipRole:
            return QString("%1: %2").arg(_name, getData(index, Qt::DisplayRole).toString());

        default:
            break;
    }

    return QVariant();
}

bool String::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    if (static_cast<Column>(index.column()) != Column::Value)
        return Item::setData(index, value, role);

    switch (role)
    {
        case Qt::EditRole:
        case Qt::DisplayRole:
        {
            if (value.toString() != _value) {
                _value = value.toString();
                _modified++;

                return true;
            }
            
            break;
        }


        default:
            break;
    }

    return false;
}

void String::accept(Visitor* visitor) const
{
    visitor->visitTreeItem(this);
}

}