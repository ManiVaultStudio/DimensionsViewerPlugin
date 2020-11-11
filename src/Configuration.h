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

public: // Columns and rows

    /** Model item columns */
    enum class Column {
        Type,
        Index,
        DatasetName,
        DataName,
        Subsets,
        SelectionStamp,

        Start   = Type,
        End     = SelectionStamp,
        Count   = End + 1
    };

    /** Maps column name to column enum and vice versa */
    static QMap<QString, Column> const columns;

    /** Get string representation of column enum */
    static QString getColumnTypeName(const Column& column) {
        return columns.key(column);
    }

    /** Get enum representation from column type name */
    static Column getColumnTypeEnum(const QString& columnName) {
        return columns[columnName];
    }

    /** Model item rows */
    enum class Row {
        Channels,
        Global,
        DifferentialProfile,
        Miscellaneous,

        Start   = Channels,
        End     = Miscellaneous,
        Count   = End + 1
    };

public: // Construction

	/**
	 * Constructor
	 * @param parent Parent model item
	 * @param datasetName Name of the primary dataset
	 * @param dataName Name of the primary data
	 */
	Configuration(ModelItem* parent, const QString& datasetName, const QString& dataName);

public: // ModelIndex: Model

    /** Returns the number of columns in the item */
    int columnCount() const override;

    /**
     * Returns the item flags for the given model index
     * @param index Model index
     * @return Item flags for the index
     */
    Qt::ItemFlags getFlags(const QModelIndex& index) const override;

    /**
     * Get data role
     * @param column Column to fetch data from
     * @param role Data role
     * @return Data in variant form
     */
    QVariant getData(const std::int32_t& column, const std::int32_t& role) const override;

    /**
     * Sets data
     * @param index Model index
     * @param value Data value in variant form
     * @param role Data role
     * @return Model indices that are affected by the operation
     */
    QModelIndexList setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role = Qt::EditRole) override;

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

public: // Spec

    /** Update configuration spec */
    void updateSpec();

	/** Get configuration spec */
	QVariantMap getSpec() const;

    /** Get modification time stamp */
    std::int32_t getModified() const;

private: // Internal

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
    QVariantMap             _spec;                          /** Specification for use in JS visualization client (Vega) */

protected:
    static std::int32_t maxNoDimensions;
    static std::int32_t noConfigurations;

    friend class ConfigurationsModel;
    friend class Configurations;
    friend class Channels;
};