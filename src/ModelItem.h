#pragma once

#include <QObject>
#include <QModelIndex>

class DimensionsViewerPlugin;

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
    ModelItem(ModelItem* parent = nullptr);

public: // MVC

    /**
     * Returns the number of rows in the model
     * @param parentIndex Parent index
     */
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const = 0;

    /** Returns the number of columns */
    virtual int getColumnCount() const = 0;

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

    /**
     * Returns the model index belonging to the given model row and column
     * @param row Model row
     * @param column Model column
     * @param parent Parent model index
     * @return Model index for the given model row and column
     */
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const = 0;

    /**
     * Returns the parent model index
     * @param index Model index
     * @return Parent model index for the given model index
     */
    virtual QModelIndex parent(const QModelIndex& index) const = 0;

public: // Hierarchy

    /**
     * Returns a child model item by index
     * @param index Index of the child model item
     * @return Model item at index
     */
    virtual ModelItem* getChild(const int& index) const = 0;

    /** Returns the number of children */
    virtual int getChildCount() const = 0;

    /** Returns the parent */
    ModelItem* getParent();

protected:
    ModelItem*      _parent;		/** Parent model item */

protected:
    static DimensionsViewerPlugin* dimensionsViewerPlugin;

    friend class DimensionsViewerPlugin;
};