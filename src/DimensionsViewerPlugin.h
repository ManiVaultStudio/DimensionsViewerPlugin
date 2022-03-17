#pragma once

#include <ViewPlugin.h>

#include "LayersModel.h"

#include <PointData.h>

#include <QMap>
#include <QStringList>
#include <QStandardItemModel>

using hdps::plugin::ViewPluginFactory;
using hdps::plugin::ViewPlugin;

class DimensionsViewerWidget;
class SettingsWidget;
class SettingsAction;

namespace hdps {
    namespace gui {
        class DropWidget;
    }
}

class DimensionsViewerPlugin : public ViewPlugin
{
    Q_OBJECT

public:
    DimensionsViewerPlugin(const PluginFactory* factory);

    void init() override;

public:
    /** Returns a pointer to the core interface */
    hdps::CoreInterface* getCore() { return _core; }

    LayersModel& getLayersModel();

public: // Action getters

    SettingsAction& getConfigurationAction() { return *_settingsAction; }

private:
    DimensionsViewerWidget*     _dimensionsViewerWidget;        /** HTML dimensions viewer (Vega) */
    LayersModel                 _layersModel;
    SettingsAction*             _settingsAction;
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

    /** Returns the plugin icon */
    QIcon getIcon() const override;

    DimensionsViewerPlugin* produce();

    hdps::DataTypes supportedDataTypes() const override;
};
