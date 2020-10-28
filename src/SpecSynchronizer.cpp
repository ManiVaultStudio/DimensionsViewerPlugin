#include "SpecSynchronizer.h"
#include "DimensionsViewerPlugin.h"

#include <QDebug>

SpecSynchronizer::SpecSynchronizer(DimensionsViewerPlugin* dimensionsViewerPlugin) :
	_dimensionsViewerPlugin(dimensionsViewerPlugin),
    _spec(),
    _vegaIsBusy(false)
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

    qDebug() << "updateData";
    _spec = configurationsModel.getSelectedConfiguration()->getSpec();

    //emit specChanged(selectedConfigurationSpec);
}

QVariantMap SpecSynchronizer::getSpec()
{
    return _spec;
}

void SpecSynchronizer::beginVegaEmbed()
{
    qDebug() << "beginVegaEmbed";

    _vegaIsBusy = true;
}

void SpecSynchronizer::endVegaEmbed()
{
    qDebug() << "endVegaEmbed";

    _vegaIsBusy = false;

    _spec = QVariantMap();
}