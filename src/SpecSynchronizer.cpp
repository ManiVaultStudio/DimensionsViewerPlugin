#include "SpecSynchronizer.h"
#include "DimensionsViewerPlugin.h"
#include "Configuration.h"

#include <QDebug>

SpecSynchronizer::SpecSynchronizer(DimensionsViewerPlugin* dimensionsViewerPlugin) :
	_dimensionsViewerPlugin(dimensionsViewerPlugin)
{
    auto& configurationsModel = _dimensionsViewerPlugin->getModel();

    QObject::connect(&configurationsModel, &ConfigurationsModel::dataChanged, [this, &configurationsModel](const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/) {
        const auto selectedRows = configurationsModel.getSelectionModel().selectedRows();

        if (selectedRows.isEmpty())
            return;

        if (selectedRows.first().row() != begin.row())
            return;

        //configurationsModel.getSelectedConfiguration()->updateSpec();

        //_spec = configurationsModel.getSelectedConfiguration()->getSpec();
    });

    QObject::connect(&configurationsModel.getSelectionModel(), &QItemSelectionModel::selectionChanged, [this, &configurationsModel](const QItemSelection& selected, const QItemSelection& deselected) {
        const auto selectedRows = configurationsModel.getSelectionModel().selectedRows();

        if (selectedRows.isEmpty())
            return;

        emit selectionChanged();
    });
}

QVariantMap SpecSynchronizer::getSpec(const int& modified)
{
    auto selectedConfiguration = _dimensionsViewerPlugin->getModel().getSelectedConfiguration();

    if (selectedConfiguration != nullptr && selectedConfiguration->getModified() > modified) {
        selectedConfiguration->updateSpec();
        return selectedConfiguration->getSpec();
    }

    return QVariantMap();
}