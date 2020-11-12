#pragma once

#include <QObject>
#include <QMap>
#include <QColor>

/**
 * Styling utility class
 *
 * @author T. Kroes
 */
class Styling {

public: // Columns and rows

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

    /** Default constructor */
    Styling();

public: // Operators

    Styling& operator=(const Styling& other)
    {
        _lineTypeProfile    = other._lineTypeProfile;
        _lineTypeRange      = other._lineTypeRange;
        _opacity            = other._opacity;
        //_color              = other._color;

        return *this;
    }

public:

    /** Get line type names */
    QStringList getLineTypeNames() const;

private:
    LineType    _lineTypeProfile;       /** Line type for drawing data profile */
    LineType    _lineTypeRange;         /** Line type for drawing data range */
    float       _opacity;               /** Opacity for data range */
    QColor      _color;                 /** Color */

    friend class Channel;
};