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

    /** TODO */
    enum Column {
        Name,
        Enabled,
        DatasetNames1,
        DatasetNames2,
        DatasetName1,
        DatasetName2,

        Start = Name,
        End = Name
    };

    /** Maps column type name to column type enum and vice versa */
    static QMap<QString, Column> const columns;

    /** Get string representation of column type enumeration */
    static QString getColumnTypeName(const Column& column) {
        return columns.key(column);
    }

    /** Get enum representation from column type name */
    static int getColumnTypeEnum(const QString& columnName) {
        return columns[columnName];
    }

public: // Get/set data roles

    static QMap<Column, std::function<QVariant(DifferentialProfile* differentialProfile)>> const getEditRoles;
    static QMap<Column, std::function<QVariant(DifferentialProfile* differentialProfile)>> const getDisplayRoles;
    static QMap<Column, std::function<QModelIndexList(DifferentialProfile* differentialProfile, const QVariant& value, const QModelIndex& index)>> const setEditRoles;

protected: // Construction

    /** TODO */
	DifferentialProfile(ModelItem* parent);

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

protected: // Miscellaneous

    /** Returns parent configuration model item */
    Configuration* getConfiguration() const;

private:
    bool            _enabled;                       /** Whether to show the differential profile in the viewer */
    QStringList     _profileDatasetNames[2];        /** Profile 1-2 dataset names (for differential profile) */
    QString         _profileDatasetName[2];         /** Profile 1-2 selected dataset name (for differential profile) */

    friend class Configuration;
};