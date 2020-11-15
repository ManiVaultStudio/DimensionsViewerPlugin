#include "TreeItemWidget.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

QAbstractItemModel* TreeItemWidget::WidgetMapper::model = nullptr;

DimensionsViewerPlugin* TreeItemWidget::dimensionsViewerPlugin = nullptr;

TreeItemWidget::TreeItemWidget(QWidget* parent) :
	QWidget(parent),
    _modelIndex(),
    _widgetMappers()
{
}

void TreeItemWidget::setModelIndex(const QPersistentModelIndex& modelIndex)
{
    _modelIndex = modelIndex;
}

ConfigurationsModel& TreeItemWidget::getModel()
{
    return dimensionsViewerPlugin->getModel();
}

QItemSelectionModel& TreeItemWidget::getSelectionModel()
{
    return getModel().getSelectionModel();
}

QModelIndex TreeItemWidget::getModelIndex() const
{
    return _modelIndex;
}

void TreeItemWidget::setData(const std::int32_t& column, const QVariant& value, const std::int32_t& role /*= Qt::EditRole*/)
{
    getModel().setData(getModelIndex().siblingAtColumn(column), value, role);
}

void TreeItemWidget::addWidgetMapper(const QString& name, const QSharedPointer<WidgetMapper>& widgetMapper)
{
    _widgetMappers[name] = widgetMapper;
}

QSharedPointer<TreeItemWidget::WidgetMapper> TreeItemWidget::getWidgetMapper(const QString& name)
{
    return _widgetMappers[name];
}

void TreeItemWidget::setDimensionsViewerPlugin(DimensionsViewerPlugin* dimensionsViewerPlugin)
{
    TreeItemWidget::dimensionsViewerPlugin = dimensionsViewerPlugin;

    WidgetMapper::model = &dimensionsViewerPlugin->getModel();
}