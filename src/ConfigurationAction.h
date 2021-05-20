#pragma once

#include "ChannelAction.h"

class ConfigurationAction : public PluginAction
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

protected:
    QVector<ChannelAction*>     _channels;
    hdps::gui::StandardAction   _showAdvancedSettings;
    hdps::gui::StandardAction   _showDimensionNames;
};