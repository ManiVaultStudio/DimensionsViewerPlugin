#pragma once

#include "DimensionsAction.h"
#include "SubsamplingAction.h"
#include "ChannelsAction.h"

#include "event/EventListener.h"

class ConfigurationAction : public PluginAction, public hdps::EventListener
{
protected:

    class Widget : public hdps::gui::WidgetActionWidget {
    public:
        Widget(QWidget* parent, ConfigurationAction* configurationAction);
    };

    QWidget* getWidget(QWidget* parent, const std::int32_t& widgetFlags) override {
        return new ConfigurationAction::Widget(parent, this);
    };

public:
	ConfigurationAction(DimensionsViewerPlugin* dimensionsViewerPlugin);

    DimensionsAction& getDimensionsAction() { return _dimensionsAction; }
    SubsamplingAction& getSubsamplingAction() { return _subsamplingAction; }
    ChannelsAction& getChannelsAction() { return _channelsAction; }

    QVariantMap getSpec();

    std::int32_t getModified() const { return _spec["modified"].toInt(); }
    void setModified() { _spec["modified"] = _spec["modified"].toInt() + 1; }

    void updateSecondaryDatasetNames();
    void loadDataset(const QString& datasetName);
    QString getLoadedDataset();
    bool isLoading() const;
    bool isLoaded() const;

protected:
    std::int32_t getNumDimensions(const QString& datasetName) const;
    QStringList getCompatibleDatasetNames(const QString& datasetName) const;

protected:
    DimensionsAction    _dimensionsAction;
    SubsamplingAction   _subsamplingAction;
    ChannelsAction      _channelsAction;
    QVariantMap         _spec;
    bool                _isLoading;

    friend class ChannelAction;
};