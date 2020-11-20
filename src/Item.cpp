#include "Item.h"

#include <QAbstractItemModel>

namespace tree {

QAbstractItemModel* Item::model = nullptr;

const QMap<QString, Item::Column> Item::children = {
    { "Name", Item::Column::Name },
    { "Value", Item::Column::Value },
    { "Type", Item::Column::Type },
    { "Modified", Item::Column::Modified },
    { "UUID", Item::Column::UUID },
    { "Flags", Item::Column::Flags }
};

Item::Item(Item* parent, const QString& type, const QString& name) :
    QObject(parent),
    _modelIndex(),
    _name(name),
    _type(type),
    _modified(-1),
    _uuid(QUuid::createUuid()),
    _flags(Qt::ItemFlag::NoItemFlags),
    _parent(parent),
    _children()
{
}

void Item::setModel(QAbstractItemModel* model)
{
    Q_ASSERT(model != nullptr);

    Item::model = model;
}

Qt::ItemFlags Item::getFlags(const QModelIndex& index) const
{
    return _flags;
}

QVariant Item::getData(const QModelIndex& index, const int& role) const
{
    return getData(static_cast<Column>(index.column()), role);
}

QVariant Item::getData(const Column& column, const int& role) const
{
    switch (role)
    {
        case Qt::EditRole: {

            switch (column)
            {
                case Column::Name:
                    return _name;

                case Column::Value:
                    break;

                case Column::Type:
                    return _type;

                case Column::Modified:
                    return _modified;

                case Column::UUID:
                    return _uuid;

                case Column::Flags:
                    return QVariant::fromValue(_flags);

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole: {

            switch (column)
            {
                case Column::Name:
                    return getData(column, Qt::EditRole);

                case Column::Type:
                    return getData(column, Qt::EditRole);

                case Column::Value:
                    break;

                case Column::Modified:
                    return QString::number(getData(column, Qt::EditRole).toInt());

                case Column::UUID:
                    return getData(column, Qt::EditRole).toUuid().toString();

                case Column::Flags:
                    break;

                default:
                    break;
            }

            break;
        }

        case Qt::ToolTipRole:
        {
            const auto tooltip = [&column](const QString& value) {
                return QString("%1: %2").arg(getColumnTypeName(static_cast<Column>(column)), value);
            };

            switch (static_cast<Column>(column))
            {
                case Column::Name:
                case Column::Type:
                case Column::Value:
                case Column::UUID:
                case Column::Modified:
                    break;

                default:
                    break;
            }

            break;
        }

        default:
            break;
    }

    return QVariant();
}

void Item::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    const auto column = static_cast<Column>(index.column());

    switch (role)
    {
        case Qt::EditRole:
        {
            switch (column)
            {
                case Column::Name:
                    _name = value.toString();
                    break;

                case Column::Type:
                    break;
                case Column::Value:
                case Column::UUID:
                case Column::Modified:
                    break;

                case Column::Flags:
                    _flags = value.value<Qt::ItemFlags>();
                    break;

                default:
                    break;
            }

            break;
        }

        default:
            break;
    }
}

void Item::setData(const Column& column, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    model->setData(_modelIndex.sibling(_modelIndex.row(), static_cast<std::int32_t>(column)), value, role);
}

QModelIndex Item::getModelIndex() const
{
    return _modelIndex;
}

void Item::setModelIndex(const QModelIndex& modelIndex)
{
    Q_ASSERT(Item::model != nullptr);

    _modelIndex = modelIndex;

    for (auto child : getChildren())
        child->setModelIndex(Item::model->index(getChildIndex(child), 0, _modelIndex));
}

QModelIndex Item::getSiblingAtColumn(const std::uint32_t& column) const
{
    return getModelIndex().siblingAtColumn(column);
}

Item::Children Item::getChildren() const
{
    return _children;
}

Item* Item::getChild(const int& index) const
{
    return _children.at(index);
}

int Item::getChildCount() const
{
    return _children.count();
}

int Item::getChildIndex() const
{
    if (_parent)
        return _parent->getChildIndex(const_cast<Item*>(this));

    return 0;
}

int Item::getChildIndex(Item* child) const
{
    return _children.indexOf(child);
}

Item* Item::getParent()
{
    return _parent;
}

bool Item::isLeaf() const
{
    return getChildCount() == 0;
}

void Item::setFlag(const Qt::ItemFlag& flag, const bool& set /*= true*/)
{
    auto flags = _flags;

    flags.setFlag(flag, set);

    setData(Column::Flags, QVariant::fromValue(flags));
}

void Item::unsetFlag(const Qt::ItemFlag& flag)
{
    setFlag(flag, false);
}

}