#pragma once

#include "Common.h"

#include <QObject>
#include <QModelIndex>

class DimensionsViewerPlugin;
class ConfigurationsModel;
class CoreInterface;

namespace hdps {
    class CoreInterface;
}

/**
 * Model item class
 *
 * @author Thomas Kroes
 */
class ModelItem : public QObject
{
    Q_OBJECT

public: // Aliases

    using AffectedColumns = QVector<std::int32_t>;

public: // Construction

    /**
     * Constructor
     * @param parent Parent model item
     */
    ModelItem(const QString& name, ModelItem* parent = nullptr);

public: // Model

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

public: // Hierarchy

    /**
     * Returns a model item node by index
     * @param index Index of the child model item
     * @return Model item at index
     */
    virtual ModelItem* getChild(const int& index) const;

    /** Returns the number of children */
    virtual int getChildCount() const;

    /**
     * Returns the child index
     * @param child Pointer to child model item
     */
    virtual int getChildIndex(ModelItem* child) const;

    /** Returns the child index of this model item w.r.t. its parent */
    int getChildIndex() const;

    /** Returns the parent */
    ModelItem* getParent();

    /** Returns whether the model item is a leaf node */
    bool isLeaf() const;

public: // Miscellaneous

    /** Get the configurations model */
    static ConfigurationsModel* getConfigurationsModel();

protected:

    /**
     * Sets the pointer to the dimensions viewer plugin
     * @param Pointer to the dimensions viewer plugin
     */
    static void setDimensionsViewerPlugin(DimensionsViewerPlugin* dimensionsViewerPlugin);

    /** Get HDPS core pointer */
    static hdps::CoreInterface* getCore();

protected:
    QString         _name;          /** Name */
    ModelItem*      _parent;		/** Parent model item */

    static std::int32_t maxNoColumns;

private:
    static DimensionsViewerPlugin* dimensionsViewerPlugin;

    friend class DimensionsViewerPlugin;
};