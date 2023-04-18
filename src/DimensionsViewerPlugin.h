#pragma once

#include <ViewPlugin.h>

#include "LayersModel.h"

#include <PointData/PointData.h>

using hdps::plugin::ViewPluginFactory;
using hdps::plugin::ViewPlugin;

class DimensionsViewerWidget;
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

    /**
     * Load one (or more datasets in the view)
     * @param datasets Dataset(s) to load
     */
    void loadData(const hdps::Datasets& datasets) override;

public:
    /** Returns a pointer to the core interface */
    hdps::CoreInterface* getCore() { return _core; }

    LayersModel& getLayersModel();

public: // Action getters

    SettingsAction& getSettingsAction() { return *_settingsAction; }

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
    Q_PLUGIN_METADATA(IID   "nl.BioVault.DimensionsViewerPlugin"
                      FILE  "DimensionsViewerPlugin.json")
    
public:
    DimensionsViewerPluginFactory() {}
    ~DimensionsViewerPluginFactory() {}

    /**
     * Get plugin icon
     * @param color Icon color for flat (font) icons
     * @return Icon
     */
    QIcon getIcon(const QColor& color = Qt::black) const override;

    /**
     * Produces the plugin
     * @return Pointer to the produced plugin
     */
    DimensionsViewerPlugin* produce() override;

    /**
     * Get the data types that the plugin supports
     * @return Supported data types
     */
    hdps::DataTypes supportedDataTypes() const override;

    /**
     * Get plugin trigger actions given \p datasets
     * @param datasets Vector of input datasets
     * @return Vector of plugin trigger actions
     */
    PluginTriggerActions getPluginTriggerActions(const hdps::Datasets& datasets) const override;
};
