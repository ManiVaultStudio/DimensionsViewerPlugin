#include "SpecSynchronizer.h"
#include "DimensionsViewerPlugin.h"
#include "ConfigurationAction.h"

#include <QDebug>

SpecSynchronizer::SpecSynchronizer(DimensionsViewerPlugin* dimensionsViewerPlugin) :
	_dimensionsViewerPlugin(dimensionsViewerPlugin)
{
}

QVariantMap SpecSynchronizer::getSpec()
{
    return _dimensionsViewerPlugin->getConfigurationAction().getSpec();
}