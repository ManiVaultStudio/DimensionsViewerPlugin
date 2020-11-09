#pragma once

#include "ModelItem.h"

class Configuration;

#include <QObject>
#include <QVector>

/**
 * Differential profile class
 *
 * @author T. Kroes
 */
class DifferentialProfile : public ModelItem {

public: // Columns

    /** Data columns */
    enum class Columns {
        Name,
        Enabled,
        DatasetNames1,
        DatasetNames2,
        DatasetName1,
        DatasetName2,

        Start = Name,
        End = DatasetName2
    };

    /** Maps column name to column enum and vice versa */
    static QMap<QString, Columns> const columns;

    /** Get string representation of column enum */
    static QString getColumnTypeName(const Columns& column) {
        return columns.key(column);
    }

    /** Get enum representation from column type name */
    static Columns getColumnTypeEnum(const QString& columnName) {
        return columns[columnName];
    }

public: // Get/set data roles

    static QMap<Columns, std::function<QVariant(DifferentialProfile* differentialProfile)>> const getEditRoles;
    static QMap<Columns, std::function<QVariant(DifferentialProfile* differentialProfile)>> const getDisplayRoles;
    static QMap<Columns, std::function<QModelIndexList(DifferentialProfile* differentialProfile, const QModelIndex& index, const QVariant& value)>> const setEditRoles;

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

protected: // Miscellaneous

    /** Returns parent configuration model item */
    Configuration* getConfiguration() const;

private:
    bool            _enabled;                       /** Whether to show the differential profile in the viewer */
    QStringList     _profileDatasetNames[2];        /** Profile 1-2 dataset names (for differential profile) */
    QString         _profileDatasetName[2];         /** Profile 1-2 selected dataset name (for differential profile) */

    friend class Configuration;
};