#pragma once

#include "PluginAction.h"

class DimensionsViewerPlugin;

class MiscellaneousAction : public PluginAction
{
public:
    class Widget : public PluginAction::Widget {
    public:
        Widget(QWidget* parent, MiscellaneousAction* miscellaneousAction);

    protected:
        QVBoxLayout     _layout;
    };

public:
    MiscellaneousAction(DimensionsViewerPlugin* dimensionsViewerPlugin);

    QWidget* createWidget(QWidget* parent) override {
        return new Widget(parent, this);
    }

private:
    hdps::gui::StandardAction   _advancedSettingsEnabled;
    hdps::gui::StandardAction   _showDimensionNames;
};