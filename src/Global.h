#pragma once

#include "ModelItem.h"
#include "Profile.h"

#include <QObject>
#include <QModelIndexList>

/**
 * Global settings class
 *
 * @author T. Kroes
 */
class Global : public ModelItem {

public: // Columns and rows

    /** Data columns */
    enum class Column {
        Name,
        Enabled,

        Start   = Name,
        End     = Enabled,
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

protected: // Construction

    /**
     * Constructor
     * @param parent Parent model item
     */
	Global(ModelItem* parent);

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

protected:
    bool        _enabled;       /** Whether whether global settings are on or off */
    Profile     _profile;       /** Global profile settings */

    friend class Configuration;
    friend class Channel;
};