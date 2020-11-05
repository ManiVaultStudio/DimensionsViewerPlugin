#include "ModelItem.h"

DimensionsViewerPlugin* ModelItem::dimensionsViewerPlugin = nullptr;

ModelItem::ModelItem(ModelItem* parent /*= nullptr*/) :
    QObject(parent),
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