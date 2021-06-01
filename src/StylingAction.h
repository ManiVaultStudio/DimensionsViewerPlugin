#pragma once

#include "PluginAction.h"

class ChannelAction;

class StylingAction : public PluginAction
{
public:
    class Widget : public PluginAction::Widget {
    public:
        Widget(QWidget* parent, StylingAction* stylingAction);

    private:
        QHBoxLayout     _layout;
    };

    class PopupWidget : public PluginAction::PopupWidget {
    public:
        PopupWidget(QWidget* parent, StylingAction* stylingAction);

    private:
        QGridLayout     _layout;
    };

public:
    StylingAction(ChannelAction* channelAction);

    QWidget* createWidget(QWidget* parent) override {
        return new Widget(parent, this);
    }

    hdps::gui::StandardAction& getShowRangeAction() { return _showRangeAction; }
    hdps::gui::ColorAction& getColorAction() { return _colorAction; }
    hdps::gui::DecimalAction& getOpacityAction() { return _opacityAction; }

protected:
    hdps::gui::StandardAction   _showRangeAction;
    hdps::gui::ColorAction      _colorAction;
    hdps::gui::DecimalAction    _opacityAction;
};