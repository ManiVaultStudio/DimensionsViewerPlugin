#pragma once

#include "DimensionsAction.h"
#include "LayersAction.h"
#include "SubsamplingAction.h"

#include <event/EventListener.h>

using namespace hdps;

class SettingsAction : public PluginAction, public hdps::EventListener
{
protected:

    class Widget : public hdps::gui::WidgetActionWidget {
    public:
        Widget(QWidget* parent, SettingsAction* settingsAction);
    };

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new SettingsAction::Widget(parent, this);
    };

public:
    SettingsAction(DimensionsViewerPlugin* dimensionsViewerPlugin);

    QVariantMap getSpec();

    std::int32_t getModified() const { return _spec["modified"].toInt(); }
    void setModified() { _spec["modified"] = _spec["modified"].toInt() + 1; }

    void updateSecondaryDatasetNames();
    void loadDataset(const QString& datasetName);
    bool isLoading() const;
    bool isLoaded() const;

protected:
    Datasets getCompatibleDatasets(const QString& datasetName) const;

public: // Action getters

    LayersAction& getLayersAction() { return _layersAction; }
    DimensionsAction& getDimensionsAction() { return _dimensionsAction; }
    SubsamplingAction& getSubsamplingAction() { return _subsamplingAction; }

protected:
    LayersAction        _layersAction;
    DimensionsAction    _dimensionsAction;
    SubsamplingAction   _subsamplingAction;
    QVariantMap         _spec;
    bool                _isLoading;

    friend class ChannelAction;
};