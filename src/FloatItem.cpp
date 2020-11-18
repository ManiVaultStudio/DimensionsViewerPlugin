#include "FloatItem.h"
#include "Visitor.h"

FloatItem::FloatItem(TreeItem* parent, const QString& name, const float& value /*= 0.0f*/) :
    TreeItem(parent, "Float", name),
    _value(value)
{
}

Qt::ItemFlags FloatItem::getFlags(const QModelIndex& index) const
{
    return Qt::ItemIsEnabled;
}

QVariant FloatItem::getData(const QModelIndex& index, const int& role) const
{
    if (static_cast<Column>(index.column()) < Column::Value)
        return TreeItem::getData(index, role);
    
    switch (role)
    {
        case Qt::EditRole:
            return _value;

        case Qt::DisplayRole:
            return QString::number(_value, 'f', 2);

        case Qt::ToolTipRole:
            return QString("%1: %2").arg(_name, getData(index, Qt::DisplayRole).toString());

        default:
            break;
    }

    return QVariant();
}

void FloatItem::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    if (static_cast<Column>(index.column()) < Column::Value) {
        TreeItem::setData(index, value, role);
        return;
    }

    switch (role)
    {
        case Qt::EditRole:
            _value = value.toFloat();
            break;

        case Qt::DisplayRole:
            break;

        default:
            break;
    }
}

void FloatItem::accept(Visitor* visitor) const
{
    visitor->visitFloatItem(this);
}