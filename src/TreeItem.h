#pragma once

#include "Common.h"

#include <QObject>
#include <QModelIndex>
#include <QUuid>

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

public: // Columns and rows

    /** Tree item columns */
    enum class Column {
        Type,                           /** Type of tree item */
        Name,                           /** Name of tree item */
        Enabled,                        /** Whether the tree item is enabled or not */
        Modified,                       /** Last modified integer stamp */
        UUID,                           /** Universal unique identifier */
        Value,                          /** Value */

        _Start  = Type,
        _End    = Value,
        _Count  = _End + 1
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

public: // Construction

    /**
     * Constructor
     * @param type Type of the tree item
     * @param name Name of the tree item
     * @param parent Parent tree item
     */
    TreeItem(const QString& type, const QString& name, TreeItem* parent = nullptr);

public: // Model API

    /** Set number of columns */
    void setNumColumns(const std::uint32_t& numColumns) const;

    /**
     * Returns the item flags for the given model index
     * @param index Model index
     * @return Item flags for the index
     */
    virtual Qt::ItemFlags getFlags(const QModelIndex& index) const;

    /**
     * Get data
     * @param index Model index
     * @param role Data role
     * @return Data in variant form
     */
    QVariant getData(const QModelIndex& index, const int& role) const;

    /**
     * Set data
     * @param index Model index
     * @param value Data value in variant form
     * @param role Data role
     */
    virtual void setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role = Qt::EditRole) = 0;

    /**
     * Get data
     * @param column Data column
     * @param role Data role
     * @return Data in variant form
     */
    virtual QVariant getData(const std::int32_t& column, const std::int32_t& role) const;

    /** Get number of columns */
    virtual std::uint32_t getColumnCount() const = 0;

    /** Gets model index */
    QModelIndex getModelIndex() const;

    /**
     * Get sibling at \p column
     * @param column Column
     * @return Model index
     */
    QModelIndex getSiblingAtColumn(const std::uint32_t& column) const;

    /** Gets persistent model index */
    void setModelIndex(const QModelIndex& modelIndex);

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

public: // Visitor API

    /** Accept visitor */
    virtual void accept(Visitor* visitor) const = 0;

public: // Miscellaneous

    /** Get the configurations model */
    static ConfigurationsModel* getModel();

    /** Sets the tree item dirty (increase the modified timer with 1) */
    void setModified() {
        _modified++;
    }

    /** Gets modified time */
    std::int32_t getModified() const {
        return _modified;
    }

    /** Get unique identifier */
    QUuid getUuid() const {
        return _uuid;
    }

protected:

    /**
     * Sets the pointer to the dimensions viewer plugin
     * @param Pointer to the dimensions viewer plugin
     */
    static void setDimensionsViewerPlugin(DimensionsViewerPlugin* dimensionsViewerPlugin);

    /** Get HDPS core pointer */
    static hdps::CoreInterface* getCore();

signals:

    /**
     * Signals that the tree item data changed
     * @param modelIndex Model index that changed
     */
    void dataChanged(const QModelIndex& modelIndex);

protected:
    QPersistentModelIndex   _modelIndex;    /** Persistent model index */
    QString                 _type;          /** Type */
    QString                 _name;          /** Display name */
    bool                    _enabled;       /** Whether the tree item is enabled or not */
    std::int32_t            _modified;      /** Modified time */
    QUuid                   _uuid;          /** Unique identifier */
    TreeItem*               _parent;        /** Parent tree item */

private:
    static DimensionsViewerPlugin* dimensionsViewerPlugin;

    friend class DimensionsViewerPlugin;
};