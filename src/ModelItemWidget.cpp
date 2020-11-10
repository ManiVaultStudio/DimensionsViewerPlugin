#include "ModelItemWidget.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

QAbstractItemModel* ModelItemWidget::WidgetMapper::model = nullptr;

DimensionsViewerPlugin* ModelItemWidget::dimensionsViewerPlugin = nullptr;

ModelItemWidget::ModelItemWidget(QWidget* parent) :
	QWidget(parent),
    _modelIndex(),
    _widgetMappers()
{
}

void ModelItemWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    _modelIndex = modelIndex;
}

ConfigurationsModel& ModelItemWidget::getModel()
{
    return dimensionsViewerPlugin->getModel();
}

QItemSelectionModel& ModelItemWidget::getSelectionModel()
{
    return getModel().getSelectionModel();
}

QModelIndex ModelItemWidget::getModelIndex() const
{
    return _modelIndex;
}

void ModelItemWidget::setData(const std::int32_t& column, const QVariant& value)
{
    getModel().setData(getModelIndex().siblingAtColumn(column), value);
}

void ModelItemWidget::addWidgetMapper(const QString& name, const QSharedPointer<WidgetMapper>& widgetMapper)
{
    _widgetMappers[name] = widgetMapper;
}

QSharedPointer<ModelItemWidget::WidgetMapper> ModelItemWidget::getWidgetMapper(const QString& name)
{
    return _widgetMappers[name];
}

void ModelItemWidget::setDimensionsViewerPlugin(DimensionsViewerPlugin* dimensionsViewerPlugin)
{
    ModelItemWidget::dimensionsViewerPlugin = dimensionsViewerPlugin;

    WidgetMapper::model = &dimensionsViewerPlugin->getModel();
}