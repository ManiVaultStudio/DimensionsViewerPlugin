#include "IntegralItem.h"
#include "Visitor.h"

IntegralItem::IntegralItem(TreeItem* parent, const QString& name, const std::int32_t& value /*= 0*/) :
    TreeItem(parent, "Integral", name),
    _value(value)
{
}

Qt::ItemFlags IntegralItem::getFlags(const QModelIndex& index) const
{
    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QVariant IntegralItem::getData(const QModelIndex& index, const int& role) const
{
    if (static_cast<Column>(index.column()) != Column::Value)
        return TreeItem::getData(index, role);
    
    switch (role)
    {
        case Qt::EditRole:
            return _value;

        case Qt::DisplayRole:
            return QString::number(_value);

        case Qt::ToolTipRole:
            return QString("%1: %2").arg(_name, getData(index, Qt::DisplayRole).toString());

        default:
            break;
    }

    return QVariant();
}

void IntegralItem::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    if (static_cast<Column>(index.column()) != Column::Value) {
        TreeItem::setData(index, value, role);
        return;
    }

    switch (role)
    {
        case Qt::EditRole:
            _value = value.toInt();
            break;

        case Qt::DisplayRole:
            break;

        default:
            break;
    }
}

void IntegralItem::accept(Visitor* visitor) const
{
    visitor->visitIntegralItem(this);
}