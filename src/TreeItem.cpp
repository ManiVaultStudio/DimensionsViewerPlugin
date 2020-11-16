#include "TreeItem.h"
#include "DimensionsViewerPlugin.h"
#include "ConfigurationsModel.h"

#include "CoreInterface.h"

std::int32_t TreeItem::maxNoColumns = 25;
DimensionsViewerPlugin* TreeItem::dimensionsViewerPlugin = nullptr;

const QMap<QString, TreeItem::Column> TreeItem::columns = {
    { "Type", TreeItem::Column::Type },
    { "Name", TreeItem::Column::Name },
    { "Enabled", TreeItem::Column::Enabled },
    { "Modified", TreeItem::Column::Modified },
    { "UUID", TreeItem::Column::UUID }
};

TreeItem::TreeItem(const QString& type, const QString& name, TreeItem* parent /*= nullptr*/) :
    QObject(parent),
    _type(type),
    _name(name),
    _enabled(true),
    _modified(-1),
    _uuid(QUuid::createUuid()),
    _parent(parent)
{
    setNumColumns(to_ul(Column::_Count));
}

void TreeItem::setNumColumns(const std::uint32_t& numColumns) const
{
    ConfigurationsModel::noColumns = std::max(ConfigurationsModel::noColumns, numColumns);
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

                default:
                    break;
            }

            break;
        }

        case Qt::DisplayRole: {

            switch (static_cast<Column>(column))
            {
                case TreeItem::Column::Type:
                    return getData(column, Qt::EditRole);

                case TreeItem::Column::Name:
                    return getData(column, Qt::EditRole);

                case TreeItem::Column::Enabled:
                    return getData(column, Qt::EditRole).toBool() ? "on" : "off";

                case TreeItem::Column::UUID:
                    return getData(column, Qt::EditRole).toUuid().toString();

                case TreeItem::Column::Modified:
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
                case Column::Enabled:
                    return QString("%1: %2").arg(getData(to_ul(TreeItem::Column::Name), Qt::DisplayRole).toString(), getData(column, Qt::DisplayRole).toString());

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
                case TreeItem::Column::Type:
                    break;

                case TreeItem::Column::Name:
                {
                    _name = value.toString();

                    break;
                }

                case TreeItem::Column::Enabled:
                {
                    _enabled = value.toBool();

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