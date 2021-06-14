#pragma once

#include "ChannelAction.h"

#include "event/EventListener.h"

class ConfigurationAction : public PluginAction, public hdps::EventListener
{
public:
    using Channels = QVector<ChannelAction*>;

protected: // Widget

    class Widget : public PluginAction::Widget {
    public:
        Widget(QWidget* parent, ConfigurationAction* configurationAction);

    protected:
        QHBoxLayout     _layout;
        QGroupBox       _channelsGroupBox;
        QVBoxLayout     _channelsGroupBoxLayout;
        QGroupBox       _miscellaneousGroupBox;
        QVBoxLayout     _miscellaneousGroupBoxLayout;
    };

    QWidget* getWidget(QWidget* parent, const Widget::State& widgetType = Widget::State::Standard) override {
        return new ConfigurationAction::Widget(parent, this);
    };

public:
	ConfigurationAction(DimensionsViewerPlugin* dimensionsViewerPlugin);

    QVector<ChannelAction*> getChannels() { return _channels; }
    std::int32_t getNumChannels() const { return _channels.count(); }

    hdps::gui::ToggleAction& getInteractiveAction() { return _interactiveAction; }
    hdps::gui::ToggleAction& getShowDimensionNamesAction() { return _showDimensionNamesAction; }

    QVariantMap getSpec();

    std::int32_t getModified() const { return _spec["modified"].toInt(); }
    void setModified() { _spec["modified"] = _spec["modified"].toInt() + 1; }

protected:
    QVector<ChannelAction*>     _channels;
    hdps::gui::ToggleAction     _interactiveAction;
    hdps::gui::ToggleAction     _showDimensionNamesAction;
    QVariantMap				    _spec;

    friend class ChannelAction;
};