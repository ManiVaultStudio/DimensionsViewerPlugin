#pragma once

#include <ViewPlugin.h>

#include "ConfigurationsModel.h"
#include "Channel.h"

#include "PointData.h"

#include <QMap>
#include <QStringList>

using hdps::plugin::ViewPluginFactory;
using hdps::plugin::ViewPlugin;

namespace hdps {
    namespace gui {
        class DropWidget;
    }
}

class ViewerWidget;
class ConfigurationWidget;

class DimensionsViewerPlugin : public ViewPlugin
{
    Q_OBJECT

public:
    DimensionsViewerPlugin();

    void init() override;

public:

    /** Returns the configurations model */
    ConfigurationsModel& getModel() { return _model; }

    /** Returns a pointer to the core interface */
    hdps::CoreInterface* getCore() { return _core; }

private:
    ConfigurationsModel     _model;                 /** Configurations model */
    ViewerWidget*           _viewerWidget;          /** HTML dimensions viewer (Vega) */
    ConfigurationWidget*    _configurationWidget;   /** Configuration widget */
    hdps::gui::DropWidget*  _dropWidget;            /** Drop widget */
};

class DimensionsViewerPluginFactory : public ViewPluginFactory
{
    Q_INTERFACES(hdps::plugin::ViewPluginFactory hdps::plugin::PluginFactory)
        Q_OBJECT
        Q_PLUGIN_METADATA(IID   "lumc.lkeb.DimensionsViewerPlugin"
            FILE  "DimensionsViewerPlugin.json")

public:
    DimensionsViewerPluginFactory() {}
    ~DimensionsViewerPluginFactory() {}

    DimensionsViewerPlugin* produce();
};
