#pragma once

#include <ViewPlugin.h>

#include "Common.h"

#include "Dimensions.h"

#include "PointData.h"

#include <QStringListModel>
#include <QItemSelectionModel>

using hdps::plugin::ViewPluginFactory;
using hdps::plugin::ViewPlugin;

class DimensionsViewerWidget;
class SettingsWidget;

class DimensionsViewerPlugin : public ViewPlugin
{
	Q_OBJECT

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

	/** Returns the indices of the selected points */
	std::vector<std::uint32_t> selectedIndices() const;

public:
	
	/** Get dimensions */
	Dimensions& getDimensions() { return _dimensions; };

	/** Returns the datasets model (string list with names of the datasets) */
	QStringListModel& getDatasetsModel() { return _datasetsModel; }

public: // GUI

	QSize sizeHint() const override;

signals:

private:
	QStringListModel			_datasetsModel;
	DimensionsViewerWidget*		_dimensionsViewerWidget;
	SettingsWidget*				_settingsWidget;
	Dimensions					_dimensions;
	Points*						_points;
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
