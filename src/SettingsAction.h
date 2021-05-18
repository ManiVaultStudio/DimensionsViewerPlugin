#pragma once

#include "PluginAction.h"

#include "event/EventListener.h"

class DimensionsViewerPlugin;
class ConfigurationAction;

class SettingsAction : public PluginAction, hdps::EventListener
{
public:
    using Configurations = QMap<QString, ConfigurationAction*>;

public:
    class Widget : public PluginAction::Widget {
    public:
        Widget(QWidget* parent, SettingsAction* settingsAction);

    protected:
        QVBoxLayout                         _layout;
        hdps::gui::OptionAction::Widget     _currentDatasetWidget;
    };

public:
    SettingsAction(DimensionsViewerPlugin* dimensionsViewerPlugin);

    QWidget* createWidget(QWidget* parent) override {
        return new Widget(parent, this);
    }

    void selectConfiguration(const QString& datasetName);

    Configurations& getConfigurations() { return _configurations; };
    ConfigurationAction* getConfiguration(const QString& datasetName) { return _configurations[datasetName]; };

protected:
    DimensionsViewerPlugin*     _dimensionsViewerPlugin;
    hdps::gui::OptionAction     _currentDataset;
    Configurations              _configurations;
};