#include "SpecSynchronizer.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

SpecSynchronizer::SpecSynchronizer(DimensionsViewerPlugin* dimensionsViewerPlugin) :
	_dimensionsViewerPlugin(dimensionsViewerPlugin)
{
    QObject::connect(&_dimensionsViewerPlugin->getConfigurationsModel(), &ConfigurationsModel::dataChanged, this, &SpecSynchronizer::updateData);
}

void SpecSynchronizer::updateData(const QModelIndex& begin, const QModelIndex& end, const QVector<int>& roles /*= QVector<int>()*/)
{
    auto& configurationsModel = _dimensionsViewerPlugin->getConfigurationsModel();

    const auto selectedRows = configurationsModel.getSelectionModel().selectedRows();

    if (selectedRows.isEmpty())
        return;

    if (selectedRows.first().row() != begin.row())
        return;

    //configurationsModel.getSelectedConfiguration()->updateSpec();

    //_spec = configurationsModel.getSelectedConfiguration()->getSpec();
}

QVariantMap SpecSynchronizer::getSpec(const int& modified)
{
    auto selectedConfiguration = _dimensionsViewerPlugin->getConfigurationsModel().getSelectedConfiguration();

    if (selectedConfiguration != nullptr && selectedConfiguration->getModified() > modified) {
        selectedConfiguration->updateSpec();
        return selectedConfiguration->getSpec();
    }

    return QVariantMap();
}