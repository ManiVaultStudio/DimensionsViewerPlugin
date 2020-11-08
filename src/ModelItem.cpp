#include "ModelItem.h"
#include "DimensionsViewerPlugin.h"
#include "ConfigurationsModel.h"

#include "CoreInterface.h"

DimensionsViewerPlugin* ModelItem::dimensionsViewerPlugin = nullptr;

ModelItem::ModelItem(const QString& name, ModelItem* parent /*= nullptr*/) :
    QObject(parent),
    _name(name),
    _parent(parent)
{
}

int ModelItem::getChildIndex() const
{
    if (_parent)
        return _parent->getChildIndex(const_cast<ModelItem*>(this));

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

ConfigurationsModel* ModelItem::getConfigurationsModel()
{
    return &dimensionsViewerPlugin->getConfigurationsModel();
}

void ModelItem::setDimensionsViewerPlugin(DimensionsViewerPlugin* dimensionsViewerPlugin)
{
    ModelItem::dimensionsViewerPlugin = dimensionsViewerPlugin;
}

hdps::CoreInterface* ModelItem::getCore()
{
    return dimensionsViewerPlugin->getCore();
}