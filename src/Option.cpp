#include "Option.h"
#include "Visitor.h"

namespace tree {

Option::Option(Item* parent, const QString& name, const std::int32_t& value /*= 0*/, const QStringList& options /*= QStringList()*/) :
    Item(parent, "Option", name),
    _value(value),
    _options(options)
{
}

QVariant Option::getData(const QModelIndex& index, const int& role) const
{
    const auto column = index.column();

    switch (role)
    {
        case Qt::EditRole:
        {
            if (column == static_cast<std::int32_t>(Item::Column::Value))
                return _value;

            if (column == static_cast<std::int32_t>(Column::Options))
                return _options;

            break;
        }

        case Qt::DisplayRole:
        {
            if (column == static_cast<std::int32_t>(Item::Column::Value))
                return QString::number(getData(index, Qt::EditRole).toInt());

            if (column == static_cast<std::int32_t>(Column::Options))
                return getData(index, Qt::EditRole).toStringList().join(", ");

            break;
        }

        default:
            break;
    }

    return Item::getData(index, role);
}

void Option::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    Item::setData(index, value, role);

    const auto column = index.column();

    switch (role)
    {
        case Qt::EditRole:
        {
            if (column == static_cast<std::int32_t>(Item::Column::Value)) {
                _value = value.toInt();
                break;
            }

            if (column == static_cast<std::int32_t>(Column::Options)) {
                _options = value.toStringList();
                break;
            }

            break;
        }

        case Qt::DisplayRole:
        {
            if (column == static_cast<std::int32_t>(Item::Column::Value)) {
               const auto option = value.toString();

               if (_options.contains(option))
                   _value = _options.indexOf(option);

               break;
            }

            if (column == static_cast<std::int32_t>(Column::Options)) {
                _options = value.toString().split(", ");

                break;
            }

            break;
        }

        default:
            break;
    }
}

void Option::accept(Visitor* visitor) const
{
    visitor->visitTreeItem(this);
}

}