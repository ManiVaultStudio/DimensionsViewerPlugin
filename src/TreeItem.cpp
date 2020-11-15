#include "TreeItem.h"
#include "DimensionsViewerPlugin.h"
#include "ConfigurationsModel.h"

#include "CoreInterface.h"

std::int32_t TreeItem::maxNoColumns = 25;
DimensionsViewerPlugin* TreeItem::dimensionsViewerPlugin = nullptr;

TreeItem::TreeItem(const QString& type, TreeItem* parent /*= nullptr*/) :
    QObject(parent),
    _type(type),
    _parent(parent),
    _modified(-1)
{
}

QVariant TreeItem::getData(const QModelIndex& index, const int& role) const
{
    return getData(index.column(), role);
}

QModelIndex TreeItem::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
    return getModel()->index(row, column, parent);
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