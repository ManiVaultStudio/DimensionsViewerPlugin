#pragma once

#include "ChannelAction.h"

#include "event/EventListener.h"

class ConfigurationAction : public PluginAction, public hdps::EventListener
{
public:
    using Channels = QVector<ChannelAction*>;

public:
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

public:
	ConfigurationAction(DimensionsViewerPlugin* dimensionsViewerPlugin);

    QWidget* createWidget(QWidget* parent) override {
        return new Widget(parent, this);
    }

    QVector<ChannelAction*> getChannels() { return _channels; }
    std::int32_t getNumChannels() const { return _channels.count(); }

    hdps::gui::StandardAction& getInteractiveAction() { return _interactiveAction; }
    hdps::gui::StandardAction& getShowAdvancedSettingsAction() { return _showAdvancedSettingsAction; }
    hdps::gui::StandardAction& getShowDimensionNamesAction() { return _showDimensionNamesAction; }

    QVariantMap getSpec();

    std::int32_t getModified() const { return _spec["modified"].toInt(); }

protected:
    QVector<ChannelAction*>     _channels;
    hdps::gui::StandardAction   _interactiveAction;
    hdps::gui::StandardAction   _showAdvancedSettingsAction;
    hdps::gui::StandardAction   _showDimensionNamesAction;
    QVariantMap				    _spec;

    friend class ChannelAction;
};