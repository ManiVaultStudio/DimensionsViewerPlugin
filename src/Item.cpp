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

Item::Item(Item* parent, const QString& type, const QString& name, const Qt::ItemFlags& flags /*= Qt::ItemIsEnabled*/) :
    QObject(parent),
    _modelIndex(),
    _name(name),
    _type(type),
    _modified(-1),
    _uuid(QUuid::createUuid()),
    _flags(flags),
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
    const auto column = static_cast<Column>(index.column());

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
                    return static_cast<int>(_flags);

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
                    return QString::number(getData(column, Qt::EditRole).toInt());

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
                case Column::Flags:
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

QVariant Item::getData(const Column& column, const int& role) const
{
    return model->data(_modelIndex.sibling(_modelIndex.row(), static_cast<std::int32_t>(column)), role);
}

bool Item::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    const auto column = static_cast<Column>(index.column());

    switch (role)
    {
        case Qt::EditRole:
        {
            switch (column)
            {
                case Column::Name:
                {
                    const auto name = value.toString();

                    if (name != _name) {
                        _name = name;
                        return true;
                    }
                        
                    break;
                }

                case Column::Type:
                case Column::Value:
                case Column::UUID:
                case Column::Modified:
                    break;

                case Column::Flags:
                {
                    const auto flags = static_cast<Qt::ItemFlags>(value.toInt());

                    if (flags != _flags) {
                        _flags = flags;
                        return true;
                    }

                    break;
                }

                default:
                    break;
            }

            break;
        }

        default:
            break;
    }

    return false;
}

bool Item::setData(const Column& column, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    return model->setData(_modelIndex.sibling(_modelIndex.row(), static_cast<std::int32_t>(column)), value, role);
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

    initialize();
}

void Item::initialize()
{
}

QModelIndex Item::getSiblingAtColumn(const std::uint32_t& column) const
{
    return getModelIndex().siblingAtColumn(column);
}

void Item::copy(const Item* other, const QVector<Column>& columns /*= QVector<Column>{ Column::Value }*/)
{
    for (auto column : columns)
        Item::setData(column, other->getData(column, Qt::EditRole), Qt::EditRole);
}

Item::Children Item::getChildren() const
{
    return _children;
}

Item* Item::getChild(const int& index) const
{
    return _children.at(index);
}

tree::Item* Item::getChild(const QString& path)
{
    Q_ASSERT(!path.isEmpty());

    if (path.isEmpty())
        return nullptr;

    auto segments = path.split("/");

    if (segments.first() == "..") {
        segments.removeFirst();
        return _parent->getChild(segments.join("/"));
    }
        
    Item* firstChild = nullptr;

    for (auto child : _children)
        if (child->_name == segments.first())
            firstChild = child;

    if (segments.count() == 1) {
        return firstChild;
    }
    else {
        segments.removeFirst();
        return firstChild->getChild(segments.join("/"));
    }

    return nullptr;
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

QVariant Item::getValue(const Column& column /*= Column::Value*/, const int& role /*= Qt::EditRole*/) const
{
    return getData(column, role);
}

void Item::setValue(const QVariant& value, const Column& column /*= Column::Value*/, const int& role /*= Qt::EditRole*/)
{
    setData(Column::Value, value, role);
}

void Item::setFlag(const Qt::ItemFlag& flag, const bool& set /*= true*/)
{
    auto flags = _flags;

    flags.setFlag(flag, set);

    setData(Column::Flags, static_cast<int>(flags));
}

void Item::unsetFlag(const Qt::ItemFlag& flag)
{
    setFlag(flag, false);
}

}