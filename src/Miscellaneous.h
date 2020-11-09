#pragma once

#include "ModelItem.h"

#include <QObject>
#include <QModelIndexList>

/**
 * Miscellaneous settings class
 *
 * @author T. Kroes
 */
class Miscellaneous : public ModelItem {

public: // Columns and rows

    /** Data columns */
    enum class Column {
        Name,
        ShowDimensionNames,

        Start = Name,
        End = ShowDimensionNames
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

public: // Get/set data roles

    static QMap<Column, std::function<QVariant(Miscellaneous* miscellaneous)>> const getEditRoles;
    static QMap<Column, std::function<QVariant(Miscellaneous* miscellaneous)>> const getDisplayRoles;
    static QMap<Column, std::function<QModelIndexList(Miscellaneous* miscellaneous, const QModelIndex& index, const QVariant& value)>> const setEditRoles;

protected: // Construction

    /**
     * Constructor
     * @param parent Parent model item
     */
	Miscellaneous(ModelItem* parent);

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
    ModelItem* getChild(const int& index) const override {
        return nullptr;
    };

    /** Returns the number of children */
    int getChildCount() const override {
        return 0;
    };

    /**
     * Returns the child index
     * @param child Pointer to child model item
     */
    int getChildIndex(ModelItem* child) const override {
        return 0;
    };

protected:
    bool        _showDimensionNames;    /** Whether to show dimension names in the viewer */

    friend class Configuration;
    friend class Channel;
};