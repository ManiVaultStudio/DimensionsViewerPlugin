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

public: // Getters/setters

    /** Gets line type names */
    QStringList getLineTypeNames() const;

    /** Gets profile line type */
    LineType getLineTypeProfile() const;

    /**
     * Sets profile line type
     * @param lineTypeProfile Line type for profile
     */
    void setLineTypeProfile(const LineType& lineTypeProfile);

    /** Gets range line type */
    LineType getLineTypeRange() const;

    /**
     * Sets range line type
     * @param lineTypeRange Line type for range
     */
    void setLineTypeRange(const LineType& lineTypeRange);

    /** Gets opacity */
    float getOpacity() const;

    /**
     * Sets opacity
     * @param opacity Opacity
     */
    void setOpacity(const float& opacity);

    /** Gets color */
    QColor getColor() const;

    /**
     * Sets color
     * @param color Color
     */
    void setColor(const QColor& color);

private:
    LineType    _lineTypeProfile;       /** Line type for drawing data profile */
    LineType    _lineTypeRange;         /** Line type for drawing data range */
    float       _opacity;               /** Opacity for data range */
    QColor      _color;                 /** Color */
};