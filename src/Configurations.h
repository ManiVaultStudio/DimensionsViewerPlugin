#pragma once

#include "TreeItem.h"

class Configuration;

/**
 * Tree item interface class
 *
 * @author Thomas Kroes
 */
class Configurations : public TreeItem
{
protected: // Construction

    /** Default constructor */
    Configurations();

public: // TreeItem: model API

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

    /** Get number of columns */
    std::uint32_t getColumnCount() const override {
        return 0;
    };

public: // TreeItem: hierarchy API

    /**
     * Returns a tree item node by index
     * @param index Index of the child tree item
     * @return Tree item at index
     */
    TreeItem* getChild(const int& index) const override;

    /** Returns the number of children */
    int getChildCount() const override;

    /**
     * Returns the child index
     * @param child Pointer to child tree item
     */
    int getChildIndex(TreeItem* child) const override;

public: // TreeItem: visitor API

    /** Accept visitor */
    void accept(Visitor* visitor) const override;

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