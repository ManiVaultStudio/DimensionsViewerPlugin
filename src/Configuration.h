#pragma once

#include "ModelItem.h"
#include "Channels.h"
#include "Global.h"
#include "DifferentialProfile.h"

#include <QStringList>
#include <QColor>
#include <QModelIndex>

class DimensionsViewerPlugin;

/**
 * Configuration class
 *
 * @author T. Kroes
 */
class Configuration : public ModelItem
{
public: // Constants

    /** The number of channels in the configuration */
    static const std::int32_t noChannels = 3;

public: // Columns

    /** Data columns */
    enum class Column {
        Name,                       /** TODO */
        Index,                      /** Index */
        DatasetName,                /** TODO */
        DataName,                   /** TODO */
        Subsets,                    /** The subset(s) parameter of the first dataset */
        SelectionStamp,             /** Auxiliary column for triggering synchronization */
        ShowDimensionNames,         /** Whether to show dimension names in the viewer */

        Start = Name,               /** Column start */
        End = ShowDimensionNames    /** Column end */
    };

    /** Maps column name to column enum and vice versa */
    static QMap<QString, Column> const columns;

	/** Get column name from column enum */
	static QString getColumnName(const Column& column) {
        return columns.key(column);
	}

    /** Get column enum from column name */
    static Column getColumnEnum(const QString& columnName) {
        return columns[columnName];
    }

    /** TODO */
    enum class Child {
        Channels,                       /** Channels model item */
        Global,                         /** Global settings model item */
        DifferentialProfileSettings,    /** Differential profile settings model item */
        MiscellaneousSettings,          /** Miscellaneous settings model item */

        Start = Channels,               /** Column start */
        End = MiscellaneousSettings     /** Column end */
    };

public: // Get/set data roles

    static QMap<Column, std::function<QVariant(Configuration* configuration, const QModelIndex& index)>> const getEditRoles;
    static QMap<Column, std::function<QVariant(Configuration* configuration, const QModelIndex& index)>> const getDisplayRoles;
    static QMap<Column, std::function<QModelIndexList(Configuration* configuration, const QVariant& value, const QModelIndex& index)>> const setEditRoles;

public: // Construction

	/**
	 * Constructor
	 * @param parent Parent model item
	 * @param datasetName Name of the primary dataset
	 * @param dataName Name of the primary data
	 */
	Configuration(ModelItem* parent, const QString& datasetName, const QString& dataName);

public: // ModelIndex: MVC

    /** Returns the number of columns in the item */
    int columnCount() const override;

    /**
     * Returns the item flags for the given model index
     * @param index Model index
     * @return Item flags for the index
     */
    Qt::ItemFlags getFlags(const QModelIndex& index) const override;

    /**
     * Returns the data for the given model index and data role
     * @param index Model index
     * @param role Data role
     * @return Data in variant form
     */
    QVariant getData(const QModelIndex& index, const int& role) const override;

    /**
     * Sets the data value for the given model index and data role
     * @param index Model index
     * @param value Data value in variant form
     * @param role Data role
     * @return Model indices that are affected by the operation
     */
    QModelIndexList setData(const QModelIndex& index, const QVariant& value, const int& role) override;

public: // ModelIndex: Hierarchy

    /**
     * Returns a model item node by index
     * @param index Index of the child model item
     * @return Model item at index
     */
    ModelItem* getChild(const int& index) const override;

    /** Returns the number of children */
    int getChildCount() const override;

    /**
     * Returns the child index
     * @param child Pointer to child model item
     */
    int getChildIndex(ModelItem* child) const override;

public: // Miscellaneous

    /** Get channels model item */
    const Channels* getChannels() const;

    /** Get global model item */
    const Global* getGlobal() const;

    /** Get differential profile model item */
    const DifferentialProfile* getDifferentialProfile() const;

    /** Returns the subset names */
    QStringList getSubsets() const { return _subsets; }

	/**
	 * Returns whether dataset with name \p datasetName exists in the configuration
	 * @param datasetName Name of the dataset to search for
	 */
	bool hasDataset(const QString& datasetName) const;

public: // Spec

    /** Update configuration spec */
    void updateSpec();

	/** Get configuration spec */
	QVariantMap getSpec() const;

    /** Get modification time stamp */
    std::int32_t getModified() const;

private: // Internal

    /** Returns whether a differential profile can be shown */
    bool canShowDifferentialProfile() const;

    /**
     * Updates the differential profile settings
     * @return Columns that are affected by the operation
     */
     //AffectedColumns updateDifferentialProfile();

protected:
    std::int32_t            _index;                         /** TODO */
    QString                 _datasetName;                   /** TODO */
    QString					_dataName;			            /** Name of the points data */
    Channels		        _channels;                      /** TODO */
    Global                  _global;                        /** TODO */
    DifferentialProfile     _differentialProfile;           /** TODO */
    QStringList		        _subsets;                       /** Subsets of the primary dataset (selected in the first channel) */
    bool                    _showDimensionNames;            /** Whether to show dimension names in the viewer */
    QVariantMap             _spec;                          /** Specification for use in JS visualization client (Vega) */

protected:
    static std::int32_t maxNoDimensions;
    static std::int32_t noConfigurations;

    friend class ConfigurationsModel;
    friend class Configurations;
    friend class Channels;
};