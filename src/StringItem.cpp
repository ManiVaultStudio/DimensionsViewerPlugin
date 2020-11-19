#include "StringItem.h"
#include "Visitor.h"

StringItem::StringItem(TreeItem* parent, const QString& name, const QString& value /*= ""*/) :
    TreeItem(parent, "String", name),
    _value(value)
{
}

Qt::ItemFlags StringItem::getFlags(const QModelIndex& index) const
{
    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QVariant StringItem::getData(const QModelIndex& index, const int& role) const
{
    if (static_cast<Column>(index.column()) != Column::Value)
        return TreeItem::getData(index, role);
    
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

void StringItem::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    if (static_cast<Column>(index.column()) != Column::Value) {
        TreeItem::setData(index, value, role);
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

void StringItem::accept(Visitor* visitor) const
{
    visitor->visitStringItem(this);
}