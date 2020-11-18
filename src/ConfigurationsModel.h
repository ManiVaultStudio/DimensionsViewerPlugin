#pragma once

#include "Configurations.h"

#include <QAbstractItemModel>
#include <QItemSelectionModel>

class DimensionsViewerPlugin;

/**
 * Configurations model
 *
 * Class for storing configurations in a Qt abstract item model
 *
 * @author T. Kroes
 */
class ConfigurationsModel : public QAbstractItemModel
{
	Q_OBJECT

public: // Extend roles

    /** User roles */
    enum Role {
        IconFontRole = Qt::UserRole,
        IconFontCharacterRole
    };

public: // Alias(es)

    /** Rule function executed when data changes */
    using RuleFunction = std::function<void(const QPersistentModelIndex& index)>;

public: // Construction

    /**
     * Constructor
     * @param dimensionsViewerPlugin Pointer to an instance of the dimensions viewer plugin
     */
	ConfigurationsModel(DimensionsViewerPlugin* dimensionsViewerPlugin);

public: // Obligatory overrides

	/**
	 * Returns the number of rows in the model
	 * @param parent Parent index
	 */
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;

	/**
	 * Returns the number of columns in the model
	 * @param parent Parent index
	 */
	int	columnCount(const QModelIndex& parent = QModelIndex()) const override;

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

    /**
     * Returns the model index belonging to the given model row and column
     * @param row Model row
     * @param column Model column
     * @param parent Parent model index
     * @return Model index for the given model row and column
     */
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

    /**
     * Returns the parent model index
     * @param index Model index
     * @return Parent model index for the given model index
     */
     QModelIndex parent(const QModelIndex& index) const override;

public: // Dataset management

	/**
	 * Add a dataset to the model
	 * @param datasetName Name of the dataset
	 */
	void addDataset(const QString& datasetName);

public: // Row selection

    /**
     * Select configuration by index
     * @param row Row index
     */
    void selectRow(const std::int32_t& row);

    /**
     * Select configuration by datasetName
     * @param datasetName Dataset name
     */
    void selectRow(const QString& datasetName);

public: // Miscellaneous

	/** Returns the selection model */
	QItemSelectionModel& getSelectionModel() { return _selectionModel; }

    /** Returns the dataset names */
    QStringList getDatasetNames() const { return _datasetNames; }

    /** TODO */
    Configuration* getConfiguration(const QModelIndex& index) const;

        /** TODO */
	Configuration* getSelectedConfiguration() const;

    /** Get configurations */
    const Configurations& getConfigurations() const;

private:
    TreeItem* getItem(const QModelIndex& index) const;

signals:

    /**
     * Signals that a configuration has changed
     * @param configuration Configuration that changed
     */
    void configurationChanged(const Configuration* configuration);

private:
    DimensionsViewerPlugin*     _dimensionsViewerPlugin;		/** Pointer to dimensions viewer plugin instance */
    Configurations              _configurations;				/** Configurations data source */
    QItemSelectionModel         _selectionModel;				/** Selection model */
    QStringList                 _datasetNames;                  /** Dataset names */

    static std::uint32_t noColumns;

    friend class Configurations;
    friend class TreeItem;
};