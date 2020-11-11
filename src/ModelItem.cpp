#include "ModelItem.h"
#include "DimensionsViewerPlugin.h"
#include "ConfigurationsModel.h"

#include "CoreInterface.h"

std::int32_t ModelItem::maxNoColumns = 25;
DimensionsViewerPlugin* ModelItem::dimensionsViewerPlugin = nullptr;

ModelItem::ModelItem(const QString& type, ModelItem* parent /*= nullptr*/) :
    QObject(parent),
    _type(type),
    _parent(parent)
{
}

QVariant ModelItem::getData(const QModelIndex& index, const int& role) const
{
    return getData(index.column(), role);
}

QModelIndex ModelItem::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
    return getModel()->index(row, column, parent);
}

ModelItem* ModelItem::getChild(const int& index) const
{
    return nullptr;
}

int ModelItem::getChildCount() const
{
    return 0;
}

int ModelItem::getChildIndex() const
{
    if (_parent)
        return _parent->getChildIndex(const_cast<ModelItem*>(this));

    return 0;
}

int ModelItem::getChildIndex(ModelItem* child) const
{
    return 0;
}

ModelItem* ModelItem::getParent()
{
    return _parent;
}

bool ModelItem::isLeaf() const
{
    return getChildCount() == 0;
}

const ConfigurationsModel* ModelItem::getModel()
{
    return &dimensionsViewerPlugin->getModel();
}

void ModelItem::setDimensionsViewerPlugin(DimensionsViewerPlugin* dimensionsViewerPlugin)
{
    ModelItem::dimensionsViewerPlugin = dimensionsViewerPlugin;
}

hdps::CoreInterface* ModelItem::getCore()
{
    return dimensionsViewerPlugin->getCore();
}