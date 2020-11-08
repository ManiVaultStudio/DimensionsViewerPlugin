#pragma once

#include <QObject>
#include <QModelIndex>

class DimensionsViewerPlugin;
class ConfigurationsModel;

/**
 * Model item class
 *
 * @author Thomas Kroes
 */
class ModelItem : public QObject
{
public: // Construction

    /**
     * Constructor
     * @param parent Parent model item
     */
    ModelItem(const QString& name, ModelItem* parent = nullptr);

public: // MVC

    /** Returns the number of columns in the item */
    virtual int columnCount() const = 0;

    /**
     * Returns the item flags for the given model index
     * @param index Model index
     * @return Item flags for the index
     */
    virtual Qt::ItemFlags getFlags(const QModelIndex& index) const = 0;

    /**
     * Returns the data for the given model index and data role
     * @param index Model index
     * @param role Data role
     * @return Data in variant form
     */
    virtual QVariant getData(const QModelIndex& index, const int& role) const = 0;

    /**
     * Sets the data value for the given model index and data role
     * @param index Model index
     * @param value Data value in variant form
     * @param role Data role
     * @return Model indices that are affected by the operation
     */
    virtual QModelIndexList setData(const QModelIndex& index, const QVariant& value, const int& role) = 0;

public: // Hierarchy

    /**
     * Returns a child model item by index
     * @param index Index of the child model item
     * @return Model item at index
     */
    virtual ModelItem* getChild(const int& index) const = 0;

    /** Returns the number of children */
    virtual int getChildCount() const = 0;

    /**
     * Returns the child index
     * @param child Pointer to child model item
     */
    virtual int getChildIndex(ModelItem* child) const = 0;

    /** Returns the child index of this model item w.r.t. its parent */
    int getChildIndex() const;

    /** Returns the parent */
    ModelItem* getParent();

    /** Returns whether the model item is a leaf node */
    bool isLeaf() const;

public:

    /** Get the configurations model */
    static ConfigurationsModel* getConfigurationsModel();

protected:
    QString         _name;          /** Name */
    ModelItem*      _parent;		/** Parent model item */

protected:
    static DimensionsViewerPlugin* dimensionsViewerPlugin;

    friend class DimensionsViewerPlugin;
};