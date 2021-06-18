#pragma once

#include "DimensionsAction.h"
#include "MiscellaneousAction.h"
#include "ChannelsAction.h"

#include "event/EventListener.h"

class ConfigurationAction : public PluginAction, public hdps::EventListener
{
protected:

    class Widget : public PluginAction::Widget {
    public:
        Widget(QWidget* parent, ConfigurationAction* configurationAction);
    };

    QWidget* getWidget(QWidget* parent, const Widget::State& widgetType = Widget::State::Standard) override {
        return new ConfigurationAction::Widget(parent, this);
    };

public:
	ConfigurationAction(DimensionsViewerPlugin* dimensionsViewerPlugin);

    DimensionsAction& getDimensionsAction() { return _dimensionsAction; }
    MiscellaneousAction& getMiscellaneousAction() { return _miscellaneousAction; }
    ChannelsAction& getChannelsAction() { return _channelsAction; }

    QVariantMap getSpec();

    std::int32_t getModified() const { return _spec["modified"].toInt(); }
    void setModified() { _spec["modified"] = _spec["modified"].toInt() + 1; }

protected:
    DimensionsAction        _dimensionsAction;
    MiscellaneousAction     _miscellaneousAction;
    ChannelsAction          _channelsAction;
    QVariantMap			    _spec;

    friend class ChannelAction;
};