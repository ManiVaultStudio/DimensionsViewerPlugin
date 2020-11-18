#include "StringListItem.h"
#include "Visitor.h"

StringListItem::StringListItem(TreeItem* parent, const QString& name, const QStringList& value /*= {}*/) :
    TreeItem(parent, "StringList", name),
    _value(value)
{
}

Qt::ItemFlags StringListItem::getFlags(const QModelIndex& index) const
{
    return Qt::ItemIsEnabled;
}

QVariant StringListItem::getData(const QModelIndex& index, const int& role) const
{
    if (static_cast<Column>(index.column()) < Column::Value)
        return TreeItem::getData(index, role);
    
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

void StringListItem::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    if (static_cast<Column>(index.column()) < Column::Value) {
        TreeItem::setData(index, value, role);
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

void StringListItem::accept(Visitor* visitor) const
{
    visitor->visitStringListItem(this);
}