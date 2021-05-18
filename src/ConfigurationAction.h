#pragma once

#include "ChannelsAction.h"
#include "MiscellaneousAction.h"

class ConfigurationAction : public PluginAction
{
public:
    class Widget : public PluginAction::Widget {
    public:
        Widget(QWidget* parent, ConfigurationAction* configurationAction);

    protected:
        QHBoxLayout _layout;
    };

public:
	ConfigurationAction(DimensionsViewerPlugin* dimensionsViewerPlugin, const QString& datasetName);

    QWidget* createWidget(QWidget* parent) override {
        return new Widget(parent, this);
    }

private:
    ChannelsAction          _channels;
    MiscellaneousAction     _miscellaneous;
};