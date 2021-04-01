#pragma once

#include <ViewPlugin.h>

#include "ConfigurationsModel.h"
#include "Channel.h"

#include "PointData.h"

#include <QMap>
#include <QStringList>

using hdps::plugin::ViewPluginFactory;
using hdps::plugin::ViewPlugin;

class DimensionsViewerWidget;
class SettingsWidget;

namespace hdps {
    namespace gui {
        class DropWidget;
    }
}

class DimensionsViewerPlugin : public ViewPlugin
{
	Q_OBJECT

public: // Aliases 

	using Channels = QVector<Channel*>;

public:
	DimensionsViewerPlugin();

	void init() override;

public:
	
	/** Returns the configurations model */
	ConfigurationsModel& getConfigurationsModel() { return _configurationsModel; }

	/** Returns a pointer to the core interface */
	hdps::CoreInterface* getCore() { return _core; }

private:
	ConfigurationsModel			_configurationsModel;           /** Configurations model */
	DimensionsViewerWidget*		_dimensionsViewerWidget;        /** HTML dimensions viewer (Vega) */
	SettingsWidget*				_settingsWidget;                /** Settings widget */
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
