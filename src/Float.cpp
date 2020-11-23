#include "Float.h"
#include "Visitor.h"

namespace tree {

Float::Float(Item* parent, const QString& name, const float& value /*= 0.0f*/) :
    StandardItem(parent, "Float", name),
    _value(value)
{
}

QVariant Float::getData(const QModelIndex& index, const int& role) const
{
    if (static_cast<Column>(index.column()) != Column::Value)
        return Item::getData(index, role);
    
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

bool Float::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    if (static_cast<Column>(index.column()) != Column::Value)
        return Item::setData(index, value, role);

    switch (role)
    {
        case Qt::EditRole:
        {
            if (value.toFloat() != _value) {
                _value = value.toFloat();
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

void Float::accept(Visitor* visitor) const
{
    visitor->visitTreeItem(this);
}

}