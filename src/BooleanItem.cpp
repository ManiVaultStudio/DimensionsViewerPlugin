#include "BooleanItem.h"
#include "Visitor.h"

BooleanItem::BooleanItem(TreeItem* parent, const QString& name, const bool& value /*= true*/) :
    TreeItem(parent, "Boolean", name),
    _value(value)
{
}

Qt::ItemFlags BooleanItem::getFlags(const QModelIndex& index) const
{
    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QVariant BooleanItem::getData(const QModelIndex& index, const int& role) const
{
    if (static_cast<Column>(index.column()) != Column::Value)
        return TreeItem::getData(index, role);
    
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

void BooleanItem::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    if (static_cast<Column>(index.column()) != Column::Value) {
        TreeItem::setData(index, value, role);
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

void BooleanItem::accept(Visitor* visitor) const
{
    visitor->visitBooleanItem(this);
}