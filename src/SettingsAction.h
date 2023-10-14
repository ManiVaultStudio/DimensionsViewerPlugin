#pragma once

#include <actions/WidgetAction.h>

#include "DimensionsAction.h"
#include "LayersAction.h"
#include "SubsamplingAction.h"

using namespace mv;

class DimensionsViewerPlugin;

class SettingsAction : public WidgetAction
{
protected:

    class Widget : public mv::gui::WidgetActionWidget {
    public:
        Widget(QWidget* parent, SettingsAction* settingsAction);
    };

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new SettingsAction::Widget(parent, this);
    };

public:
    SettingsAction(DimensionsViewerPlugin& dimensionsViewerPlugin);

    DimensionsViewerPlugin& getDimensionsViewerPlugin();

    QVariantMap getSpec();

    std::int32_t getModified() const { return _spec["modified"].toInt(); }
    void setModified() { _spec["modified"] = _spec["modified"].toInt() + 1; }

public: // Action getters

    LayersAction& getLayersAction() { return _layersAction; }
    DimensionsAction& getDimensionsAction() { return _dimensionsAction; }
    SubsamplingAction& getSubsamplingAction() { return _subsamplingAction; }

protected:
    DimensionsViewerPlugin&     _dimensionsViewerPlugin;
    LayersAction                _layersAction;
    DimensionsAction            _dimensionsAction;
    SubsamplingAction           _subsamplingAction;
    QVariantMap                 _spec;
    bool                        _isLoading;

    friend class ChannelAction;
};