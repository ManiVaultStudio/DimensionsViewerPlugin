#pragma once

#include <ViewPlugin.h>

#include "PointData.h"

#include <QMap>
#include <QStringList>
#include <QStandardItemModel>

using hdps::plugin::ViewPluginFactory;
using hdps::plugin::ViewPlugin;

class DimensionsViewerWidget;
class SettingsWidget;
class ConfigurationAction;

namespace hdps {
    namespace gui {
        class DropWidget;
    }
}

class DimensionsViewerPlugin : public ViewPlugin
{
	Q_OBJECT

public:
	DimensionsViewerPlugin();

	void init() override;

public:
	/** Returns a pointer to the core interface */
	hdps::CoreInterface* getCore() { return _core; }

private:
	QWidget*		            _dimensionsViewerWidget;        /** HTML dimensions viewer (Vega) */
    ConfigurationAction*        _configurationAction;
    hdps::gui::DropWidget*      _dropWidget;
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
