#include "LayersAction.h"
#include "SettingsAction.h"
#include "DimensionsViewerPlugin.h"

#include <Application.h>

#include <QDebug>

LayersAction::LayersAction(SettingsAction& settingsAction) :
    GroupAction(&settingsAction, "Layers"),
    _settingsAction(settingsAction)
{
    setText("Channels");
    setIconByName("layer-group");
    setEnabled(false);

    addAction(&_settingsAction);

    auto& layersModel = _settingsAction.getDimensionsViewerPlugin().getLayersModel();

    const auto updateReadOnly = [this, &layersModel]() {
        setEnabled(layersModel.rowCount() > 0);
    };

    connect(&layersModel, &QAbstractListModel::rowsInserted, this, updateReadOnly);
    connect(&layersModel, &QAbstractListModel::rowsRemoved, this, updateReadOnly);

    for (auto layer : layersModel.getLayers())
        addAction(&layer->getChannelAction());

    connect(&layersModel, &QAbstractListModel::rowsInserted, this, [this, &layersModel](const QModelIndex& parent, int first, int last) {
        auto layer = static_cast<Layer*>(layersModel.index(first, 0).internalPointer());

        addAction(&layer->getChannelAction());
    });

    connect(&layersModel, &QAbstractListModel::rowsRemoved, this, [this, &layersModel](const QModelIndex& parent, int first, int last) {
        auto layer = static_cast<Layer*>(layersModel.index(first, 0).internalPointer());

        addAction(&layer->getChannelAction());
    });
}

SettingsAction& LayersAction::getSettingsAction()
{
    return _settingsAction;
}
