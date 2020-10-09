#pragma once

#include "Configuration.h"

#include <QAbstractListModel>
#include <QItemSelectionModel>

class DimensionsViewerPlugin;

/**
 * Configurations model
 *
 * Class for storing configurations in a Qt list model.
 */
class ConfigurationsModel : public QAbstractListModel
{
	Q_OBJECT

public: // Construction

	ConfigurationsModel(DimensionsViewerPlugin* dimensionsViewerPlugin);

public: // Obligatory overrides

	/**
	 * Returns the number of color maps in the model
	 * @param parentIndex Parent index
	 */
	int rowCount(const QModelIndex& parentIndex = QModelIndex()) const override;

	/**
	 * Returns model data for the given index
	 * @param index Index
	 * @param role The data role
	 */
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

public: // Dataset management

	void addDataset(const QString& datasetName);

public: // Miscellaneous

	/** Returns the selection model */
	QItemSelectionModel& getSelectionModel() { return _selectionModel; }

	/** Get configuration names */
	QStringList getConfigurationNames();

signals:

private:
	DimensionsViewerPlugin*		_dimensionsViewerPlugin;
	QVector<Configuration*>		_configurations;
	QItemSelectionModel			_selectionModel;				/** Selection model */
};