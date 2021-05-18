#pragma once

#include "PluginAction.h"
#include "ChannelAction.h"

class ChannelsAction : public PluginAction
{
public:

	using Channels = QVector<ChannelAction*>;

	static const std::int32_t noChannels = 3;

public:
    class Widget : public PluginAction::Widget {
    public:
        Widget(QWidget* parent, ChannelsAction* channelsAction);

    protected:
        QVBoxLayout _layout;
    };

public:
    ChannelsAction(DimensionsViewerPlugin* dimensionsViewerPlugin);

    QWidget* createWidget(QWidget* parent) override {
        return new Widget(parent, this);
    }

private:
    hdps::gui::StandardAction   _enabled;
    QVector<ChannelAction*>     _channels;
};