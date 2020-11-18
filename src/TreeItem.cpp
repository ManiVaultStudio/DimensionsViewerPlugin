#include "TreeItem.h"
#include "DimensionsViewerPlugin.h"
#include "ConfigurationsModel.h"

#include "CoreInterface.h"

DimensionsViewerPlugin* TreeItem::dimensionsViewerPlugin = nullptr;

const QMap<QString, TreeItem::Column> TreeItem::columns = {
    { "Name", TreeItem::Column::Name },
    { "Value", TreeItem::Column::Value },
    { "Type", TreeItem::Column::Type },
    { "Enabled", TreeItem::Column::Enabled },
    { "Modified", TreeItem::Column::Modified },
    { "UUID", TreeItem::Column::UUID }
};

TreeItem::TreeItem(TreeItem* parent, const QString& type, const QString& name) :
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

Qt::ItemFlags TreeItem::getFlags(const QModelIndex& index) const
{
    return Qt::ItemFlag::NoItemFlags;
}

QVariant TreeItem::getData(const QModelIndex& index, const int& role) const
{
    const auto column = static_cast<Column>(index.column());

    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(column))
            {
                case TreeItem::Column::Type:
                    return _type;

                case TreeItem::Column::Name:
                    return _name;

                case TreeItem::Column::Enabled:
                    return _enabled;

                case TreeItem::Column::UUID:
                    return _uuid;

                case TreeItem::Column::Modified:
                    return _modified;

                case TreeItem::Column::Value:
                    break;

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole: {

            switch (static_cast<Column>(column))
            {
                case TreeItem::Column::Type:
                    return getData(index, Qt::EditRole);

                case TreeItem::Column::Name:
                    return getData(index, Qt::EditRole);

                case TreeItem::Column::Enabled:
                    return getData(index, Qt::EditRole).toBool() ? "on" : "off";

                case TreeItem::Column::UUID:
                    return getData(index, Qt::EditRole).toUuid().toString();

                case TreeItem::Column::Modified:
                    return QString::number(getData(index, Qt::EditRole).toInt());

                case TreeItem::Column::Value:
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
                    return QString("%1: %2").arg(getData(index.siblingAtColumn(to_ul(TreeItem::Column::Name)), Qt::DisplayRole).toString(), getData(index, Qt::DisplayRole).toString());

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

void TreeItem::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    switch (role)
    {
        case Qt::EditRole: {

            const auto column = static_cast<Column>(index.column());

            switch (column)
            {
                case TreeItem::Column::Type:
                    break;

                case TreeItem::Column::Name:
                    _name = value.toString();
                    break;

                case TreeItem::Column::Enabled:
                    _enabled = value.toBool();
                    break;

                case TreeItem::Column::UUID:
                case TreeItem::Column::Modified:
                case TreeItem::Column::Value:
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

QModelIndex TreeItem::getModelIndex() const
{
    return _modelIndex;
}

QModelIndex TreeItem::getSiblingAtColumn(const std::uint32_t& column) const
{
    return getModelIndex().siblingAtColumn(column);
}

void TreeItem::setModelIndex(const QModelIndex& modelIndex)
{
    _modelIndex = modelIndex;

    for (auto child : getChildren())
        child->setModelIndex(getModel()->index(getChildIndex(child), 0, _modelIndex));
}

TreeItem::Children TreeItem::getChildren() const
{
    return _children;
}

TreeItem* TreeItem::getChild(const int& index) const
{
    return _children.at(index);
}

int TreeItem::getChildCount() const
{
    return _children.count();
}

int TreeItem::getChildIndex() const
{
    if (_parent)
        return _parent->getChildIndex(const_cast<TreeItem*>(this));

    return 0;
}

int TreeItem::getChildIndex(TreeItem* child) const
{
    return _children.indexOf(child);
}

TreeItem* TreeItem::getParent()
{
    return _parent;
}

bool TreeItem::isLeaf() const
{
    return getChildCount() == 0;
}

ConfigurationsModel* TreeItem::getModel()
{
    return &dimensionsViewerPlugin->getModel();
}

void TreeItem::setDimensionsViewerPlugin(DimensionsViewerPlugin* dimensionsViewerPlugin)
{
    TreeItem::dimensionsViewerPlugin = dimensionsViewerPlugin;
}

hdps::CoreInterface* TreeItem::getCore()
{
    return dimensionsViewerPlugin->getCore();
}