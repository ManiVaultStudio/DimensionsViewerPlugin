#pragma once

#include "ModelItem.h"

class Configuration;

#include <QObject>

/**
 * Differential profile class
 *
 * @author T. Kroes
 */
class DifferentialProfile : public ModelItem {

public: // Columns and rows

    /** Model item columns */
    enum class Column {
        Name,
        Enabled,
        DatasetNames1,
        DatasetNames2,
        DatasetName1,
        DatasetName2,

        Start   = Name,
        End     = DatasetName2,
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

protected: // Construction

    /**
     * Constructor
     * @param parent Parent model item
     */
	DifferentialProfile(ModelItem* parent);

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
     * Set data
     * @param column Data column
     * @param value Data value in variant form
     * @param role Data role
     * @return Model indices that are affected by the operation
     */
    AffectedColumns setData(const std::int32_t& column, const QVariant& value, const std::int32_t& role = Qt::EditRole) override;

public: // ModelIndex: Hierarchy

    /**
     * Returns a model item node by index
     * @param index Index of the child model item
     * @return Model item at index
     */
    ModelItem* getChild(const int& index) const override {
        return nullptr;
    }

    /** Returns the number of children */
    int getChildCount() const override {
        return 0;
    }

    /**
     * Returns the child index
     * @param child Pointer to child model item
     */
    int getChildIndex(ModelItem* child) const override {
        return 0;
    }

protected: // Miscellaneous

    /** Returns parent configuration model item */
    Configuration* getConfiguration() const;

private:
    bool            _enabled;                       /** Whether to show the differential profile in the viewer */
    QStringList     _profileDatasetNames[2];        /** Profile 1-2 dataset names (for differential profile) */
    QString         _profileDatasetName[2];         /** Profile 1-2 selected dataset name (for differential profile) */

    friend class Configuration;
};