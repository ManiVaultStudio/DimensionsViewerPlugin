#pragma once

#include "ModelItem.h"

class Configuration;

/**
 * Model item interface class
 *
 * @author Thomas Kroes
 */
class Configurations : public ModelItem
{
public: // Columns

    /** Data columns */
    enum class Column {
        Name,           /** TODO */

        Start = Name,
        End = Name
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

    static QMap<Column, std::function<QVariant(Configurations* configurations)>> const getEditRoles;
    static QMap<Column, std::function<QVariant(Configurations* configurations)>> const getDisplayRoles;
    static QMap<Column, std::function<QModelIndexList(Configurations* configurations, const QVariant& value, const QModelIndex& index)>> const setEditRoles;

protected: // Construction

    /** Default constructor */
    Configurations();

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

public: // Miscellaneous

    /**
     * Add a configuration
     * @param datasetName Name of the points dataset
     * @param dataName Name of the points data
     */
    void add(const QString& datasetName, const QString& dataName);

protected:
    QVector<Configuration*>		_configurations;		/** Child tree items */

    friend class ConfigurationsModel;
};