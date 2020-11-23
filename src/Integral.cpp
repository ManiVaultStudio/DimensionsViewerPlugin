#include "Integral.h"
#include "Visitor.h"

namespace tree {

Integral::Integral(Item* parent, const QString& name, const std::int32_t& value /*= 0*/) :
    StandardItem(parent, "Integral", name),
    _value(value)
{
}

QVariant Integral::getData(const QModelIndex& index, const int& role) const
{
    if (static_cast<Column>(index.column()) != Column::Value)
        return Item::getData(index, role);
    
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

bool Integral::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    if (static_cast<Column>(index.column()) != Column::Value)
        return Item::setData(index, value, role);

    switch (role)
    {
        case Qt::EditRole:
        {
            if (value.toInt() != _value) {
                _value = value.toInt();
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

void Integral::accept(Visitor* visitor) const
{
    visitor->visitTreeItem(this);
}

}