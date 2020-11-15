#include "TreeItem.h"
#include "DimensionsViewerPlugin.h"
#include "ConfigurationsModel.h"

#include "CoreInterface.h"

std::int32_t TreeItem::maxNoColumns = 25;
DimensionsViewerPlugin* TreeItem::dimensionsViewerPlugin = nullptr;

const QMap<QString, TreeItem::Column> TreeItem::columns = {
    { "Name", TreeItem::Column::Name },
    { "Display name", TreeItem::Column::DisplayName },
    { "UUID", TreeItem::Column::UUID },
    { "Modified", TreeItem::Column::Modified }
};

TreeItem::TreeItem(const QString& type, TreeItem* parent /*= nullptr*/) :
    QObject(parent),
    _internalName(type),
    _parent(parent),
    _uuid(),
    _modified(-1)
{
}

Qt::ItemFlags TreeItem::getFlags(const QModelIndex& index) const
{
    return Qt::ItemFlag::NoItemFlags;
}

QVariant TreeItem::getData(const QModelIndex& index, const int& role) const
{
    return getData(index.column(), role);
}

QVariant TreeItem::getData(const std::int32_t& column, const std::int32_t& role) const
{
    switch (role)
    {
        case Qt::EditRole: {

            switch (static_cast<Column>(column))
            {
                case TreeItem::Column::Name:
                    return _internalName;

                case TreeItem::Column::DisplayName:
                    return _displayName;

                case TreeItem::Column::UUID:
                    return _uuid;

                case TreeItem::Column::Modified:
                    return _modified;

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole: {

            switch (static_cast<Column>(column))
            {
                case TreeItem::Column::Name:
                    return getData(column, Qt::EditRole);

                case TreeItem::Column::DisplayName:
                    return getData(column, Qt::EditRole);

                case TreeItem::Column::UUID:
                    return getData(column, Qt::EditRole).toUuid().toString();

                case TreeItem::Column::Modified:
                    return QString::number(getData(column, Qt::EditRole).toInt());

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

QModelIndexList TreeItem::setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    QModelIndexList affectedIndices{ index };

    switch (role)
    {
        case Qt::EditRole: {

            const auto column = static_cast<Column>(index.column());

            switch (column)
            {
                case TreeItem::Column::Name:
                {
                    _internalName = value.toString();

                    break;
                }

                case TreeItem::Column::DisplayName:
                {
                    _displayName = value.toString();

                    break;
                }

                case TreeItem::Column::UUID:
                case TreeItem::Column::Modified:
                    break;

                default:
                    break;
            }

            break;
        }

        default:
            break;
    }

    return affectedIndices;
}

QModelIndex TreeItem::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
    return getModel()->index(row, column, parent);
}

QVector<TreeItem*> TreeItem::getChildren() const
{
    QVector<TreeItem*> children;

    for (int childIndex = 0; childIndex < getChildCount(); childIndex++)
        children << getChild(childIndex);

    return children;
}

TreeItem* TreeItem::getChild(const int& index) const
{
    return nullptr;
}

int TreeItem::getChildCount() const
{
    return 0;
}

int TreeItem::getChildIndex() const
{
    if (_parent)
        return _parent->getChildIndex(const_cast<TreeItem*>(this));

    return 0;
}

int TreeItem::getChildIndex(TreeItem* child) const
{
    return 0;
}

TreeItem* TreeItem::getParent()
{
    return _parent;
}

bool TreeItem::isLeaf() const
{
    return getChildCount() == 0;
}

const ConfigurationsModel* TreeItem::getModel()
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