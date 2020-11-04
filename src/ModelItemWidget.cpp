#include "ModelItemWidget.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

DimensionsViewerPlugin* ModelItemWidget::dimensionsViewerPlugin = nullptr;

ModelItemWidget::ModelItemWidget(QWidget* parent) :
	QWidget(parent),
    _modelIndex()
{
    auto& configurationsModel = dimensionsViewerPlugin->getConfigurationsModel();

    QObject::connect(&configurationsModel, &ConfigurationsModel::dataChanged, [this](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles = QVector<int>()) {
        if (topLeft.parent().isValid() && topLeft.parent() == _modelIndex)
            updateData(topLeft, bottomRight, roles);
    });
}

void ModelItemWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    _modelIndex = modelIndex;

    if (modelIndex == QModelIndex()) {
        updateData(QModelIndex(), QModelIndex());
    } else {
        const auto noColumns = dimensionsViewerPlugin->getConfigurationsModel().columnCount(modelIndex);
        updateData(modelIndex.sibling(0, 0), modelIndex.sibling(0, noColumns - 1));
    }
}