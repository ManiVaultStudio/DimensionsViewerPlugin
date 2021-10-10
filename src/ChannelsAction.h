#pragma once

#include "ChannelAction.h"

#include "event/EventListener.h"

class ConfigurationAction;

class ChannelsAction: public PluginAction, public hdps::EventListener
{
public:
    using Channels = QVector<ChannelAction*>;

protected: // Widget

    class Widget : public hdps::gui::WidgetActionWidget {
    public:
        Widget(QWidget* parent, ChannelsAction* channelsAction);
    };

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new ChannelsAction::Widget(parent, this);
    };

public:
    ChannelsAction(ConfigurationAction* configurationAction);

    QVector<ChannelAction*> getChannels() { return _channels; }
    std::int32_t getNumChannels() const { return _channels.count(); }

    ConfigurationAction* getConfigurationAction() { return _configurationAction; }

    bool canDisplay() const;

protected:
    ConfigurationAction*        _configurationAction;
    QVector<ChannelAction*>     _channels;

    friend class ChannelAction;
};