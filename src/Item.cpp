#include "Item.h"

#include <QAbstractItemModel>

namespace tree {

QAbstractItemModel* Item::model = nullptr;

const QMap<QString, Item::Column> Item::columns = {
    { "Name", Item::Column::Name },
    { "Value", Item::Column::Value },
    { "Type", Item::Column::Type },
    { "Enabled", Item::Column::Enabled },
    { "Modified", Item::Column::Modified },
    { "UUID", Item::Column::UUID }
};

Item::Item(Item* parent, const QString& type, const QString& name) :
    QObject(parent),
    _modelIndex(),
    _type(type),
    _name(name),
    _enabled(true),
    _modified(-1),
    _uuid(QUuid::createUuid()),
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
    return Qt::ItemFlag::NoItemFlags;
}

QVariant Item::getData(const QModelIndex& index, const int& role) const
{
    const auto column = static_cast<Column>(index.column());

    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(column))
            {
                case Item::Column::Type:
                    return _type;

                case Item::Column::Name:
                    return _name;

                case Item::Column::Enabled:
                    return _enabled;

                case Item::Column::UUID:
                    return _uuid;

                case Item::Column::Modified:
                    return _modified;

                case Item::Column::Value:
                    break;

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole: {

            switch (static_cast<Column>(column))
            {
                case Item::Column::Type:
                    return getData(index, Qt::EditRole);

                case Item::Column::Name:
                    return getData(index, Qt::EditRole);

                case Item::Column::Enabled:
                    return getData(index, Qt::EditRole).toBool() ? "on" : "off";

                case Item::Column::UUID:
                    return getData(index, Qt::EditRole).toUuid().toString();

                case Item::Column::Modified:
                    return QString::number(getData(index, Qt::EditRole).toInt());

                case Item::Column::Value:
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
                case Column::Type:
                case Column::Name:
                    break;

                case Column::Enabled:
                    return QString("%1: %2").arg(getData(index.siblingAtColumn(to_ul(Item::Column::Name)), Qt::DisplayRole).toString(), getData(index, Qt::DisplayRole).toString());

                case Column::Modified:
                case Column::UUID:
                case Column::Value:
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
    switch (role)
    {
        case Qt::EditRole: {

            const auto column = static_cast<Column>(index.column());

            switch (column)
            {
                case Item::Column::Type:
                    break;

                case Item::Column::Name:
                    _name = value.toString();
                    break;

                case Item::Column::Enabled:
                    _enabled = value.toBool();
                    break;

                case Item::Column::UUID:
                case Item::Column::Modified:
                case Item::Column::Value:
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

QModelIndex Item::getModelIndex() const
{
    return _modelIndex;
}

QModelIndex Item::getSiblingAtColumn(const std::uint32_t& column) const
{
    return getModelIndex().siblingAtColumn(column);
}

void Item::setModelIndex(const QModelIndex& modelIndex)
{
    Q_ASSERT(Item::model != nullptr);

    _modelIndex = modelIndex;

    for (auto child : getChildren())
        child->setModelIndex(Item::model->index(getChildIndex(child), 0, _modelIndex));
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

}