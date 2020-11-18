#include "ColorItem.h"
#include "Visitor.h"

ColorItem::ColorItem(TreeItem* parent, const QString& name, const QColor& value /*= Qt::black*/) :
    TreeItem(parent, "Color", name),
    _value(value)
{
}

Qt::ItemFlags ColorItem::getFlags(const QModelIndex& index) const
{
    return Qt::ItemIsEnabled;
}

QVariant ColorItem::getData(const QModelIndex& index, const int& role) const
{
    if (static_cast<Column>(index.column()) < Column::Value)
        return TreeItem::getData(index, role);
    
    switch (role)
    {
        case Qt::EditRole:
            return _value;

        case Qt::DisplayRole:
            return _value.name();

        case Qt::ToolTipRole:
            return QString("%1: %2").arg(_name, getData(index, Qt::DisplayRole).toString());

        default:
            break;
    }

    return QVariant();
}

void ColorItem::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    if (static_cast<Column>(index.column()) < Column::Value) {
        TreeItem::setData(index, value, role);
        return;
    }

    switch (role)
    {
        case Qt::EditRole:
            _value = value.value<QColor>();
            break;

        case Qt::DisplayRole:
            break;

        default:
            break;
    }
}

void ColorItem::accept(Visitor* visitor) const
{
    visitor->visitColorItem(this);
}