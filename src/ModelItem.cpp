#include "ModelItem.h"

DimensionsViewerPlugin* ModelItem::dimensionsViewerPlugin = nullptr;

ModelItem::ModelItem(ModelItem* parent /*= nullptr*/) :
    QObject(parent),
    _parent(parent)
{
}

ModelItem* ModelItem::getParent()
{
    return _parent;
}