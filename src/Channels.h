#pragma once

#include "ModelItem.h"

class Channel;
class Configuration;

#include <QObject>
#include <QVector>

/**
 * Channels class
 *
 * @author T. Kroes
 */
class Channels : public ModelItem {

public: // Columns and rows

    /** Model item columns */
    enum class Column {
        Name,

        Start   = Name,
        End     = Name,
        Count   = End + 1
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
    enum Row {
        Channel1,
        Channel2,
        Channel3,
        DifferentialProfile,

        Start   = Channel1,
        End     = DifferentialProfile,
        Count   = End + 1
    };

protected: // Construction

    /**
     * Constructor
     * @param parent Parent model item
     * @param datasetName Name of the primary dataset
     * @param dataName Name of the primary data
     */
	Channels(ModelItem* parent, const QString& datasetName, const QString& dataName);

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
     * Get data role
     * @param column Column to fetch data from
     * @param role Data role
     * @return Data in variant form
     */
    QVariant getData(const std::int32_t& column, const std::int32_t& role) const override;

    /**
     * Set data
     * @param column Data column
     * @param value Data value in variant form
     * @param role Data role
     * @return Model indices that are affected by the operation
     */
    AffectedColumns setData(const std::int32_t& column, const QVariant& value, const std::int32_t& role = Qt::EditRole) override;

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

protected: // Miscellaneous

    /** Returns parent configuration model item */
    const Configuration* getConfiguration() const;

public:

    /** Get number of enabled channels */
    std::int32_t getNoChannelsEnabled() const;

public: // Overloaded operators

    Channel* operator [](int i) const { return _channels[i]; }

private:
    QVector<Channel*>       _channels;

    friend class Configuration;
    friend class Channel;
};