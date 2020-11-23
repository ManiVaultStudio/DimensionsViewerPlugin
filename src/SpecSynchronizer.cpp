#include "SpecSynchronizer.h"
#include "DimensionsViewerPlugin.h"
#include "Configuration.h"

#include <QDebug>

SpecSynchronizer::SpecSynchronizer(DimensionsViewerPlugin* dimensionsViewerPlugin) :
	_dimensionsViewerPlugin(dimensionsViewerPlugin),
    _timer(),
    _specVisitor(),
    _updateInterval(20)
{
    //QObject::connect(&_timer, &QTimer::timeout, [this]() {
    //    //qDebug() << "Updating";

    //    const auto selectedConfiguration = _dimensionsViewerPlugin->getModel().getSelectedConfiguration();

    //    selectedConfiguration->accept(&_specVisitor);

    //    //qDebug() << _specVisitor.getSpec();

    //    _timer.stop();
    //});

    //QObject::connect(&_dimensionsViewerPlugin->getModel(), &ConfigurationsModel::configurationChanged, [this](const Configuration* configuration) {
    //    const auto selectedConfiguration = _dimensionsViewerPlugin->getModel().getSelectedConfiguration();

    //    if (configuration != selectedConfiguration)
    //        return;

    //    if (_timer.isActive()) {
    //        _timer.stop();

    //        
    //    }
    //    
    //    _timer.start(_updateInterval);
    //});
}

QVariantMap SpecSynchronizer::getSpec(const int& modified)
{
    return QVariantMap();
}