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
	 * Returns the number of rows in the model
	 * @param parentIndex Parent index
	 */
	int rowCount(const QModelIndex& parentIndex = QModelIndex()) const override;

	/**
	 * Returns the number of columns in the model
	 * @param parentIndex Parent index
	 */
	int	columnCount(const QModelIndex& parentIndex = QModelIndex()) const override;

	/**
	 * Returns model data for the given index
	 * @param index Index
	 * @param role The data role
	 */
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	/**
	 * Sets the data value for the given model index and data role
	 * @param index Model index
	 * @param value Data value in variant form
	 * @param role Data role
	 * @return Whether the data was properly set or not
	 */
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

	/**
	 * Returns the item flags for the given model index
	 * @param index Model index
	 * @return Item flags for the index
	 */
	Qt::ItemFlags flags(const QModelIndex &index) const override;

	/**
	 * Returns the header data for the given section, orientation and data role
	 * @param section Model section
	 * @param orientation Orientation (e.g. horizontal or vertical)
	 * @param role Data role
	 * @return Header data in variant form
	 */
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

public: // Dataset management

	void addDataset(const QString& datasetName);

public: // Miscellaneous

	/** Returns the selection model */
	QItemSelectionModel& getSelectionModel() { return _selectionModel; }

	/** Get configuration names */
	QStringList getConfigurationNames();

	/**
	 * Select a single row
	 * @param row Row identifier
	 */
	void selectRow(const std::int32_t& rowIndex);

private:

	/**
	 * Returns a configuration by model index
	 * @param index Model index of the layer to obtain
	 * @return Configuration
	 */
	Configuration* getConfiguration(const QModelIndex& index) const;

signals:

private:
	DimensionsViewerPlugin*		_dimensionsViewerPlugin;
	QVector<Configuration*>		_configurations;
	QItemSelectionModel			_selectionModel;				/** Selection model */
};