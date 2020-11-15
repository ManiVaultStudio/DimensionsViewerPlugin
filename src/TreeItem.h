#pragma once

#include "Common.h"

#include <QObject>
#include <QModelIndex>

class DimensionsViewerPlugin;
class ConfigurationsModel;
class CoreInterface;
class Visitor;

namespace hdps {
    class CoreInterface;
}

/**
 * Tree item class
 *
 * @author Thomas Kroes
 */
class TreeItem : public QObject
{
    Q_OBJECT

public: // Construction

    /**
     * Constructor
     * @param type Type of tree item
     * @param parent Parent tree item
     */
    TreeItem(const QString& type, TreeItem* parent = nullptr);

public: // Model API

    /** Returns the number of columns in the item */
    virtual int columnCount() const = 0;

    /**
     * Returns the item flags for the given model index
     * @param index Model index
     * @return Item flags for the index
     */
    virtual Qt::ItemFlags getFlags(const QModelIndex& index) const = 0;

    /**
     * Get data
     * @param index Model index
     * @param role Data role
     * @return Data in variant form
     */
    QVariant getData(const QModelIndex& index, const int& role) const;

    /**
     * Sets data
     * @param index Model index
     * @param value Data value in variant form
     * @param role Data role
     * @return Model indices that are affected by the operation
     */
    virtual QModelIndexList setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role = Qt::EditRole) = 0;

    /**
     * Get data
     * @param column Data column
     * @param role Data role
     * @return Data in variant form
     */
    virtual QVariant getData(const std::int32_t& column, const std::int32_t& role) const = 0;

    /**
     * Returns the model index belonging to the given model row and column (wraps the model index())
     * @param row Model row
     * @param column Model column
     * @param parent Parent model index
     * @return Model index for the given model row and column
     */
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;

public: // Hierarchy API

    /** Gets children */
    QVector<TreeItem*> getChildren() const;

    /**
     * Get a tree item node by index
     * @param index Index of the child tree item
     * @return Model item at index
     */
    virtual TreeItem* getChild(const int& index) const;

    /** Returns the number of children */
    virtual int getChildCount() const;

    /**
     * Returns the child index
     * @param child Pointer to child tree item
     */
    virtual int getChildIndex(TreeItem* child) const;

    /** Returns the child index of this tree item w.r.t. its parent */
    int getChildIndex() const;

    /** Returns the parent */
    TreeItem* getParent();

    /** Returns whether the tree item is a leaf node */
    bool isLeaf() const;

protected: // Visitor API

    /** Accept visitor */
    virtual void accept(Visitor* visitor) const;

public: // Miscellaneous

    /** Get the configurations model */
    static const ConfigurationsModel* getModel();

    /** Sets the tree item dirty (increase the modified timer with 1) */
    void setModified() {
        _modified++;
    }

    /** Gets modified time */
    std::int32_t getModified() const {
        return _modified;
    }

protected:

    /**
     * Sets the pointer to the dimensions viewer plugin
     * @param Pointer to the dimensions viewer plugin
     */
    static void setDimensionsViewerPlugin(DimensionsViewerPlugin* dimensionsViewerPlugin);

    /** Get HDPS core pointer */
    static hdps::CoreInterface* getCore();

protected:
    QString         _type;          /** Name */
    TreeItem*      _parent;		    /** Parent tree item */
    std::int32_t    _modified;      /** Modified time */

    static std::int32_t maxNoColumns;

private:
    static DimensionsViewerPlugin* dimensionsViewerPlugin;

    friend class DimensionsViewerPlugin;
};