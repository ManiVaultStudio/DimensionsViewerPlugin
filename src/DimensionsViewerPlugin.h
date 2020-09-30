#pragma once

#include <ViewPlugin.h>

#include "Common.h"

#include "Channel.h"

#include "PointData.h"

#include <QMap>
#include <QStringList>

using hdps::plugin::ViewPluginFactory;
using hdps::plugin::ViewPlugin;

class DimensionsViewerWidget;
class SettingsWidget;

class DimensionsViewerPlugin : public ViewPlugin
{
	Q_OBJECT

public: // Aliases 

	using Datasets = QMap<QString, QStringList>;
	using Channels = QVector<Channel*>;

public:
	DimensionsViewerPlugin();

	void init() override;

	void dataAdded(const QString name) Q_DECL_OVERRIDE;
	void dataChanged(const QString name) Q_DECL_OVERRIDE;
	void dataRemoved(const QString name) Q_DECL_OVERRIDE;
	void selectionChanged(const QString dataName) Q_DECL_OVERRIDE;

	/** Determines which data types this the image viewer is compatible with */
	hdps::DataTypes supportedDataTypes() const Q_DECL_OVERRIDE;

private:

	

public:
	
	/** Get dimensions */
	Channels& getChannels() { return _channels; };

	/** Returns the datasets */
	Datasets& getDatasets() { return _datasets; }

	/** Returns a pointer to the core interface */
	hdps::CoreInterface* getCore() { return _core; }

public: // GUI

signals:

	void datasetsChanged(const Datasets& datasets);

private:
	Datasets					_datasets;
	DimensionsViewerWidget*		_dimensionsViewerWidget;
	SettingsWidget*				_settingsWidget;
	Channels					_channels;
};

class DimensionsViewerPluginFactory : public ViewPluginFactory
{
	Q_INTERFACES(hdps::plugin::ViewPluginFactory hdps::plugin::PluginFactory)
    Q_OBJECT
    Q_PLUGIN_METADATA(IID   "lumc.lkeb.DimensionsViewerPlugin"
                      FILE  "DimensionsViewerPlugin.json")
    
public:
	DimensionsViewerPluginFactory() {}
    ~DimensionsViewerPluginFactory() {}
    
	DimensionsViewerPlugin* produce();
};
