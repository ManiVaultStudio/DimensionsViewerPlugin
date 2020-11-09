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
        Name,               /** Name of the model item */
        LineTypeProfile,    /** Line type for drawing data profile */
        LineTypeRange,      /** Line type for drawing data range */
        Opacity,            /** Opacity */
        Color,              /** Color */

        Start = Name,
        End = Color,
        Count = End + 1
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
    static QString getProfileTypeName(const LineType& lineType) {
        return lineTypes.key(lineType);
    }

    /** Get line type enum from line type name */
    static LineType getProfileTypeEnum(const QString& lineTypeName) {
        return lineTypes[lineTypeName];
    }

public: // Get/set data roles

    static QMap<Column, std::function<QVariant(Styling* styling)>> const getEditRoles;
    static QMap<Column, std::function<QVariant(Styling* styling)>> const getDisplayRoles;
    static QMap<Column, std::function<QModelIndexList(Styling* styling, const QModelIndex& index, const QVariant& value)>> const setEditRoles;

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
     * Returns the data for the given model index and data role
     * @param index Model index
     * @param role Data role
     * @return Data in variant form
     */
    QVariant getData(const QModelIndex& index, const int& role) const override;

    /**
     * Sets the data value for the given model index and data role
     * @param index Model index
     * @param value Data value in variant form
     * @param role Data role
     * @return Model indices that are affected by the operation
     */
    QModelIndexList setData(const QModelIndex& index, const QVariant& value, const int& role) override;

private:
    LineType    _lineTypeProfile;       /** Line type for drawing data profile */
    LineType    _lineTypeRange;         /** Line type for drawing data range */
    float       _opacity;               /** Opacity for data range */
    QColor      _color;                 /** Color */
};