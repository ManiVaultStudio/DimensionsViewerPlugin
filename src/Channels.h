#pragma once

#include "TreeItem.h"
#include "Profile.h"

class Channel;
class Configuration;

#include <QObject>
#include <QVector>

/**
 * Channels model item class
 *
 * @author T. Kroes
 */
class Channels : public TreeItem {

public: // Columns and rows

    /** Model item columns */
    enum class Column {
        Type,

        _Start  = Type,
        _End    = Type,
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

    /** Model item rows */
    enum class Row {
        Dataset,
        Subset1,
        Subset2,
        Differential,

        _Start  = Dataset,
        _End    = Differential,
        _Count  = _End + 1
    };

    /** Rows set alias */
    using Rows = QSet<Row>;

    /** Maps row name to row enum */
    static QMap<QString, Row> const rows;

    /** Get row name from row enum */
    static QString getRowTypeName(const Row& row) {
        return rows.key(row);
    }

    /** Get row enum from row name */
    static Row getRowTypeEnum(const QString& rowName) {
        return rows[rowName];
    }

protected: // Construction

    /**
     * Constructor
     * @param parent Parent model item
     * @param datasetName Name of the primary dataset
     * @param dataName Name of the primary data
     */
	Channels(TreeItem* parent, const QString& datasetName, const QString& dataName);

public: // TreeItem: model API

    /** Returns the number of columns in the item */
    int columnCount() const override;

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

public: // TreeItem: hierarchy API

    /**
     * Returns a model item node by index
     * @param index Index of the child model item
     * @return Model item at index
     */
    TreeItem* getChild(const int& index) const override;

    /** Returns the number of children */
    int getChildCount() const override;

    /**
     * Returns the child index
     * @param child Pointer to child model item
     */
    int getChildIndex(TreeItem* child) const override;

public: // TreeItem: visitor API

    /** Accept visitor */
    void accept(Visitor* visitor) const override;

protected: // Miscellaneous

    /** Returns parent configuration model item */
    const Configuration* getConfiguration() const;

public: // Getters

    /**
     * Get (filtered) channels
     * @param profileTypes Filter profile types
     * @param enabled Filter enabled/disabled channels
     * @return Filtered channels
     */
    QVector<Channel*> getFiltered(const Profile::ProfileTypes& profileTypes, bool* enabled = nullptr) const;

public: // Overloaded operators

    Channel* operator [](int i) const { return _channels[i]; }

protected:
    QVector<Channel*>       _channels;

    friend class Configuration;
    friend class Channel;
    friend class Differential;
};

inline uint qHash(Channels::Row key, uint seed) {
    return ::qHash(static_cast<uint>(key), seed);
}