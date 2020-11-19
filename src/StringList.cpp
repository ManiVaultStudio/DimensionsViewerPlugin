#include "StringList.h"
#include "Visitor.h"

namespace tree {

StringList::StringList(Item* parent, const QString& name, const QStringList& value /*= {}*/) :
    Item(parent, "StringList", name),
    _value(value)
{
}

Qt::ItemFlags StringList::getFlags(const QModelIndex& index) const
{
    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QVariant StringList::getData(const QModelIndex& index, const int& role) const
{
    if (static_cast<Column>(index.column()) != Column::Value)
        return Item::getData(index, role);
    
    switch (role)
    {
        case Qt::EditRole:
            return _value;

        case Qt::DisplayRole:
            return _value.join(", ");

        case Qt::ToolTipRole:
            return QString("%1: %2").arg(_name, getData(index, Qt::DisplayRole).toString());

        default:
            break;
    }

    return QVariant();
}

void StringList::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    if (static_cast<Column>(index.column()) != Column::Value) {
        Item::setData(index, value, role);
        return;
    }

    switch (role)
    {
        case Qt::EditRole:
            _value = value.toStringList();
            break;

        case Qt::DisplayRole:
            break;

        default:
            break;
    }
}

void StringList::accept(Visitor* visitor) const
{
    visitor->visitTreeItem(this);
}

}