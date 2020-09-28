#pragma once

#include <ViewPlugin.h>

#include "Common.h"

#include "Dimensions.h"

#include "PointData.h"

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

	/**
	 * Sets the current dataset name
	 * @param currentDatasetName Current dataset name
	 */
	void setCurrentDatasetName(const QString& currentDatasetName);

public:
	
	/** Get dimensions */
	Dimensions& getDimensions() { return _dimensions; };

	/** Returns the datasets */
	QStringList& getDatasets() { return _datasets; }

public: // GUI

signals:

	void datasetsChanged(const QStringList& datasets);

private:
	QStringList					_datasets;
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
