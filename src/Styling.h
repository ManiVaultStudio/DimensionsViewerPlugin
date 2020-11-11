#pragma once

#include "ModelItem.h"

#include <QObject>
#include <QMap>
#include <QColor>

/**
 * Styling model item class
 *
 * @author T. Kroes
 */
class Styling : public ModelItem {

public: // Columns and rows

    /** Model item columns */
    enum class Column {
        Type,               /** TODO */
        LineTypes,          /** Line types */
        LineTypeProfile,    /** Line type for drawing data profile */
        LineTypeRange,      /** Line type for drawing data range */
        Opacity,            /** Opacity */
        Color,              /** Color */

        Start   = Type,
        End     = Color,
        Count   = End + 1
    };

    /** Maps column name to column enum */
    static QMap<QString, Column> const columns;

    /** Get column name from column enum */
    static QString getColumnTypeName(const Column& column) {
        return columns.key(column);
    }

    /** Get column enum from column name */
    static Column getColumnTypeEnum(const QString& columnName) {
        return columns[columnName];
    }

    /** Model item rows */
    enum class Row {};

public: // Enumerations

    /** Line types */
    enum class LineType {
        Solid,          /** Display solid line */
        Dot,            /** Display dotted line */
        DashDot,        /** Display dash-dot line */

        Start = Solid,
        End = DashDot
    };

    /** Maps line type name to line type enum */
    static QMap<QString, LineType> const lineTypes;

    /** Get line type name from line type enum */
    static QString getLineTypeName(const LineType& lineType) {
        return lineTypes.key(lineType);
    }

    /** Get line type enum from line type name */
    static LineType getLineTypeEnum(const QString& lineTypeName) {
        return lineTypes[lineTypeName];
    }

public: // Construction

    /**
     * Constructor
     * @param parent Parent model item
     */
    Styling(ModelItem* parent);

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
     * Sets data
     * @param index Model index
     * @param value Data value in variant form
     * @param role Data role
     * @return Model indices that are affected by the operation
     */
    QModelIndexList setData(const QModelIndex& index, const QVariant& value, const std::int32_t& role = Qt::EditRole) override;

private:
    LineType    _lineTypeProfile;       /** Line type for drawing data profile */
    LineType    _lineTypeRange;         /** Line type for drawing data range */
    float       _opacity;               /** Opacity for data range */
    QColor      _color;                 /** Color */

    friend class Channel;
};