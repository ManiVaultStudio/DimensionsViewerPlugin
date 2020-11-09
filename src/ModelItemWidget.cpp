#include "ModelItemWidget.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

DimensionsViewerPlugin* ModelItemWidget::dimensionsViewerPlugin = nullptr;

ModelItemWidget::ModelItemWidget(QWidget* parent) :
	QWidget(parent),
    _persistentModelIndex()
{
    auto& configurationsModel = dimensionsViewerPlugin->getConfigurationsModel();

    QObject::connect(&configurationsModel, &ConfigurationsModel::dataChanged, [this](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles = QVector<int>()) {
        if (topLeft.parent().isValid() && topLeft.parent() == _persistentModelIndex.parent() && topLeft.row() == _persistentModelIndex.row())
            updateData(topLeft, bottomRight, roles);
    });
}

void ModelItemWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    _persistentModelIndex = modelIndex;

    if (modelIndex == QModelIndex()) {
        updateData(QModelIndex(), QModelIndex());
    } else {
        const auto noColumns = dimensionsViewerPlugin->getConfigurationsModel().columnCount(modelIndex);

        updateData(getModelIndex().siblingAtColumn(0), getModelIndex().siblingAtColumn(noColumns - 1));
    }
}

ConfigurationsModel& ModelItemWidget::getConfigurationsModel()
{
    return dimensionsViewerPlugin->getConfigurationsModel();
}

QItemSelectionModel& ModelItemWidget::getSelectionModel()
{
    return getConfigurationsModel().getSelectionModel();
}

QPersistentModelIndex ModelItemWidget::getSiblingModelIndex(const std::int32_t& row, const std::int32_t& column /*= 0*/)
{
    return getConfigurationsModel().index(row, column, _persistentModelIndex);
}

QModelIndex ModelItemWidget::getModelIndex() const
{
    return _persistentModelIndex;
}

void ModelItemWidget::setData(const std::int32_t& column, const QVariant& value)
{
    getConfigurationsModel().setData(getModelIndex().siblingAtColumn(column), value);
}

void ModelItemWidget::reset()
{
    updateData(QModelIndex(), QModelIndex());
}