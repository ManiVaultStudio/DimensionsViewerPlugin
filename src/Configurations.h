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
public: // ModelIndex: MVC

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
     * Returns the data for the given column and data role
     * @param index Model index
     * @param role Data role
     * @return Data in variant form
     */
    QVariant getData(const int& column, const int& role) const override;

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

    /** Get configuration names */
    QStringList getNames() const;

protected:
    QVector<Configuration*>		_configurations;		/** Child tree items */
};