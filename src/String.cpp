#include "String.h"
#include "Visitor.h"

namespace tree {

String::String(Item* parent, const QString& name, const QString& value /*= ""*/) :
    Item(parent, "String", name),
    _value(value)
{
}

Qt::ItemFlags String::getFlags(const QModelIndex& index) const
{
    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
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

void String::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    if (static_cast<Column>(index.column()) != Column::Value) {
        Item::setData(index, value, role);
        return;
    }

    switch (role)
    {
        case Qt::EditRole:
            _value = value.toString();
            break;

        case Qt::DisplayRole:
            break;

        default:
            break;
    }
}

void String::accept(Visitor* visitor) const
{
    visitor->visitTreeItem(this);
}

}