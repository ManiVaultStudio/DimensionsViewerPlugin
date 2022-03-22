#include "LayersAction.h"
#include "SettingsAction.h"
#include "DimensionsViewerPlugin.h"

#include <Application.h>

#include <QDebug>

LayersAction::LayersAction(SettingsAction& settingsAction) :
    WidgetAction(&settingsAction),
    _settingsAction(settingsAction)
{
    setText("Channels");
    setIcon(Application::getIconFont("FontAwesome").getIcon("layer-group"));
    setEnabled(false);

    auto& layersModel = _settingsAction.getDimensionsViewerPlugin().getLayersModel();

    const auto updateReadOnly = [this, &layersModel]() {
        setEnabled(layersModel.rowCount() > 0);
    };

    connect(&layersModel, &QAbstractListModel::rowsInserted, this, updateReadOnly);
    connect(&layersModel, &QAbstractListModel::rowsRemoved, this, updateReadOnly);
}

SettingsAction& LayersAction::getSettingsAction()
{
    return _settingsAction;
}

LayersAction::Widget::Widget(QWidget* parent, LayersAction* layersAction) :
    WidgetActionWidget(parent, layersAction)
{
    auto& layersModel = layersAction->getSettingsAction().getDimensionsViewerPlugin().getLayersModel();

    auto layout = new QVBoxLayout();

    setPopupLayout(layout);

    for (auto layer : layersModel.getLayers())
        layout->addWidget(layer->getChannelAction().createWidget(this));

    connect(&layersModel, &QAbstractListModel::rowsInserted, this, [this, layout, &layersModel](const QModelIndex& parent, int first, int last) {
        layout->insertWidget(first, static_cast<Layer*>(layersModel.index(first, 0).internalPointer())->getChannelAction().createWidget(this));
    });

    connect(&layersModel, &QAbstractListModel::rowsRemoved, this, [layout, layersAction](const QModelIndex& parent, int first, int last) {
        auto item = layout->takeAt(first);

        if (item->widget())
            delete item->widget();

        delete item;
    });

    update();
}
