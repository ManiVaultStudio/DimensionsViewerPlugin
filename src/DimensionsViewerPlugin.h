#pragma once

#include <ViewPlugin.h>

#include "LayersModel.h"

#include <PointData/PointData.h>

using mv::plugin::ViewPluginFactory;
using mv::plugin::ViewPlugin;

class DimensionsViewerWidget;
class SettingsAction;

namespace mv {
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
    void loadData(const mv::Datasets& datasets) override;

public:
    /** Returns a pointer to the core interface */
    mv::CoreInterface* getCore() { return _core; }

    LayersModel& getLayersModel();

public: // Action getters

    SettingsAction& getSettingsAction() { return *_settingsAction; }

private:
    DimensionsViewerWidget*     _dimensionsViewerWidget;        /** HTML dimensions viewer (Vega) */
    LayersModel                 _layersModel;
    SettingsAction*             _settingsAction;
    mv::gui::DropWidget*      _dropWidget;
};

class DimensionsViewerPluginFactory : public ViewPluginFactory
{
    Q_INTERFACES(mv::plugin::ViewPluginFactory mv::plugin::PluginFactory)
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
    mv::DataTypes supportedDataTypes() const override;

    /**
     * Get plugin trigger actions given \p datasets
     * @param datasets Vector of input datasets
     * @return Vector of plugin trigger actions
     */
    PluginTriggerActions getPluginTriggerActions(const mv::Datasets& datasets) const override;
};
