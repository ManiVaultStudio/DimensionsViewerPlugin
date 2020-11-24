#pragma once

#include "Common.h"

#include <QObject>
#include <QModelIndex>
#include <QUuid>
#include <QFlags>
#include <QDebug>

class QAbstractItemModel;

namespace tree {

class Visitor;

/**
 * Tree item base class
 *
 * @author Thomas Kroes
 */
class Item : public QObject
{
    Q_OBJECT

public:

    using Items = QVector<Item*>;

public: // Columns and rows

    /** Column enumeration */
    enum class Column {
        Name,               /** Name of tree item */
        Value,              /** Value */
        Type,               /** Type of tree item */
        Modified,           /** Last modified integer stamp */
        UUID,               /** Universal unique identifier */
        Flags,              /** Item flags */

        _Start  = Name,
        _End    = Flags,
        _Count  = _End + 1
    };

    /** Columns set alias */
    using Columns = QSet<Column>;

    /** Maps column name to column enum and vice versa */
    static QMap<QString, Column> const children;

    /** Get string representation of column enum */
    static QString getColumnTypeName(const Column& column) {
        return children.key(column);
    }

    /** Get enum representation from column type name */
    static Column getColumnTypeEnum(const QString& columnName) {
        return children[columnName];
    }

public: // Construction

    /**
     * Constructor
     * @param parent Parent tree item
     * @param type Type of the tree item
     * @param name Name of the tree item
     */
    Item(Item* parent, const QString& type, const QString& name, const Qt::ItemFlags& flags = Qt::ItemIsEnabled);

public: // Model API

    /**
     * Set model
     * @param model Model
     */
    static void setModel(QAbstractItemModel* model);

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
    virtual QVariant getData(const QModelIndex& index, const int& role) const;

    /**
     * Get data for \p column
     * @param column Data column
     * @param role Data role
     * @return Data in variant form
     */
    QVariant getData(const Column& column, const int& role) const;

    /**
     * Set data
     * @param index Model index
     * @param value Data value in variant form
     * @param role Data role
     */
    virtual bool setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role);

    /**
     * Set data for \p column
     * @param column Data column
     * @param value Data value in variant form
     * @param role Data role
     */
    bool setData(const Column& column, const QVariant& value, const std::int32_t& role);

    /** Gets model index */
    QModelIndex getModelIndex() const;

    /** Gets persistent model index */
    void setModelIndex(const QModelIndex& modelIndex);

    /** Initialization after the model index has been set */
    virtual void initialize();

    /**
     * Get sibling at \p column
     * @param column Column
     * @return Model index
     */
    QModelIndex getSiblingAtColumn(const std::uint32_t& column) const;

    /**
     * Copies data from other item
     * @param other Item to copy from
     * @param columns Columns to copy
     */
    virtual void copy(const Item* other, const QVector<Column>& columns = QVector<Column>{ Column::Value });

public: // Hierarchy API

    /** Gets children */
    Items getChildren() const;

    /**
     * Get child by \p index
     * @param index Child index
     * @return Item if found, else nullptr
     */
    Item* getChild(const int& index) const;

    /**
     * Get child by relative \p path
     * @param path Path to child (relative paths are also valid: ../Dataset/profile)
     * @return Item if found, else nullptr
     */
    Item* getChild(const QString& path);

    /**
     * Find child(ren) by relative \p path
     * @param column Search column
     * @param search Search string (relative paths and wild cards are also valid)
     * @param match Match flags (Qt::MatchExactly, Qt::MatchContains, Qt::MatchStartsWith, Qt::MatchEndsWith, Qt::MatchCaseSensitive)
     * @return Item if found, else nullptr
     */
    Items find(const Column& column, const QString& query, const Qt::MatchFlags& match = Qt::MatchExactly);

    /** Returns the number of children */
    virtual int getChildCount() const;

    /**
     * Returns the child index
     * @param child Pointer to child tree item
     */
    virtual int getChildIndex(Item* child) const;

    /** Returns the child index of this tree item w.r.t. its parent */
    int getChildIndex() const;

    /** Returns the parent */
    Item* getParent();

    /** Returns whether the tree item is a leaf node */
    bool isLeaf() const;

public: // Visitor API

    /** Accept visitor */
    virtual void accept(Visitor* visitor) const = 0;

public: // Column getter/setter

    /**
     * Get value for \p role
     * @param column Data column
     * @param role Data role
     * @return Value in variant form
     */
    QVariant getValue(const int& role = Qt::EditRole) const {
        return getData(Column::Value, role);
    }

    /**
     * Set \p value for \p role
     * @param value Value in variant form
     * @param column Data column
     * @param role Data role
     */
    void setValue(const QVariant& value, const int& role = Qt::EditRole) {
        setData(Column::Value, value, role);
    }

public: // Flags getter/setter

    /**
     * Set \p flag
     * @param flag Flag
     */
    void setFlag(const Qt::ItemFlag& flag, const bool& set = true);

    /**
     * Unset \p flag
     * @param flag Flag
     */
    void unsetFlag(const Qt::ItemFlag& flag);

signals:

    /**
     * Signals that the tree item data changed
     * @param modelIndex Model index that changed
     */
    void dataChanged(const QModelIndex& modelIndex);

protected:
    QPersistentModelIndex       _modelIndex;    /** Persistent model index */
    QString                     _name;          /** Display name */
    QString                     _type;          /** Type */
    std::int32_t                _modified;      /** Modified time */
    QUuid                       _uuid;          /** Unique identifier */
    Qt::ItemFlags               _flags;         /** UI flags */
    Item*                       _parent;        /** Parent tree item */
    Items                       _children;      /** Children */

    /** Pointer to abstract item model */
    static QAbstractItemModel* model;
};

/**
 * Print item
 * @param debug Qt debug stream
 * @param item Item to print
 * @return Qt debug stream
 */
QDebug inline operator<<(QDebug debug, const Item* item) {
    auto noQuoteDebug = qDebug().noquote();

    noQuoteDebug << item->getData(Item::Column::Name, Qt::DisplayRole).toString();
    noQuoteDebug << item->getData(Item::Column::Value, Qt::DisplayRole).toString();
    noQuoteDebug << item->getData(Item::Column::Type, Qt::DisplayRole).toString();
    //noQuoteDebug << item->getData(Item::Column::Modified, Qt::DisplayRole).toString();
    //noQuoteDebug << item->getData(Item::Column::UUID, Qt::DisplayRole).toString();
    //noQuoteDebug << item->getData(Item::Column::Flags, Qt::DisplayRole).toString();
    
    return debug;
}

/**
 * Print items
 * @param debug Qt debug stream
 * @param items Items to print
 * @return Qt debug stream
 */
QDebug inline operator<<(QDebug debug, const Item::Items& items) {

    auto noQuoteDebug = qDebug().noquote();

    for (auto item : items)
        debug << item;

    return debug;
}

/** Get scoped enum in columns set to work */
inline uint qHash(Item::Column key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}

}