#include "SpecSynchronizer.h"
#include "DimensionsViewerPlugin.h"
#include "ConfigurationAction.h"

#include <QDebug>

SpecSynchronizer::SpecSynchronizer(DimensionsViewerPlugin* dimensionsViewerPlugin) :
	_dimensionsViewerPlugin(dimensionsViewerPlugin)
{
}

QVariantMap SpecSynchronizer::getSpec(const int& modified)
{
    auto& configurationAction = _dimensionsViewerPlugin->getConfigurationAction();

    if (configurationAction.getModified() > modified)
        configurationAction.updateSpec();

    return configurationAction.getSpec();
}